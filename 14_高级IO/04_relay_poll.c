/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.26-15:47:35
 * Description: 04_relay_poll 使用poll重构relay
 */

/**
 * IO多路复用-poll相关API
 *
 * * int poll(struct pollfd *fds, nfds_t nfds, int timeout);
 * @brief 监视并等待多个文件描述符的属性变化
 * @param fds     指向一个结构体数组的第0个元素的指针
 * @param nfds    用来指定第一个参数数组元素个数
 * @param timeout 指定等待的毫秒数,无论 I/O 是否准备好,poll() 都会返回.
 *        timeout == -1 永远等待,直到事件发生
 *        timeout == 0  立即返回
 *        timeout >  0  超时设置
 * @return int 成功时,poll() 返回结构体中 revents 域不为 0 的文件描述符个数
 * 如果在超时前没有任何事件发生,poll()返回 0;
 * 失败时,poll() 返回 -1,并设置 errno 为下列值之一
 *     EBADF  一个或多个结构体中指定的文件描述符无效
 *     EFAULT fds 指针指向的地址超出进程的地址空间
 *     EINTR  请求的事件之前产生一个信号,调用可以重新发起
 *     EINVAL nfds 参数超出 PLIMIT_NOFILE 值
 *     ENOMEM 可用内存不足,无法完成请求
 *
 * * struct pollfd {
 * *     int   fd;      // 文件描述符
 * *     short events;  // 请求的事件
 * *     short revents; // 返回的事件
 * * };
 *         常量        作为events的值 最为revents的值   说明
 * 读事件:   POLLIN     √             √               普通或优先带数据可读
 *          POLLRDNORM √             √               普通数据可读
 *          POLLRDBAND √             √               优先级带数据可读
 *          POLLPRI    √             √               高优先级带数据可读
 * 写事件:   POLLOUT    √             √               普通或优先带数据可读
 *          POLLWRNORM √             √               普通数据可读
 *          POLLWRBAND √             √               优先级带数据可读
 * 错误事件: POLLERR                  √               发生错误
 *          POLLHUP                  √               发生挂起
 *          POLLNVAL                 √               描述不是打开的文件
 * 优点:
 * ? 1.poll() 不要求开发者计算最大文件描述符加一的大小
 * ? 2.poll() 在应付大数目的文件描述符的时候速度更快,相比于select
 * ? 3.它没有最大连接数的限制,原因是它是基于链表来存储的
 * ? 4.在调用函数时,只需要对参数进行一次设置就好了
 *
 * 缺点:
 * ? 1.大量的fd的数组被整体复制于用户态和内核地址空间之间,而不管这样的复制是不是有意义
 * ? 2.与select一样,poll返回后,需要轮询pollfd来获取就绪的描述符,这样会使性能下降
 * ? 3.同时连接的大量客户端在一时刻可能只有很少的就绪状态,因此随着监视的描述符数量的增长,其效率也会线性下降
 */

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TTY1 "/dev/pts/0"
#define TTY2 "/dev/pts/2"
#define BUF_SIZE 1024

enum {
    STATE_R = 1,
    STATE_W,
    STATE_E,
    STATE_T,
};

typedef struct fsm_s {
    int state;          // 状态机状态
    int sfd;            // 源文件
    int dfd;            // 目标文件
    char buf[BUF_SIZE]; // 读写缓存区大小
    int len;            // 读取成功字节数
    int pos;            // 缓冲区当前位置
    char* errstr;       // 错误来源
} fsm_t;

/* 状态机驱动 */
static void fsm_driver(fsm_t* fsm)
{
    int ret;

    switch (fsm->state) {
    case STATE_R:
        fsm->len = read(fsm->sfd, fsm->buf, BUF_SIZE);
        if (fsm->len == 0) { // 读取字节数为0进入终止态
            fsm->state = STATE_T;
        }
        else if (fsm->len < 0) {   // 读取字节数小于0
            if (errno == EAGAIN) { // 假错重读
                fsm->state = STATE_R;
            }
            else { // 真错写入错误消息,并进入异常态
                fsm->errstr = "read failed";
                fsm->state  = STATE_E;
            }
        }
        else {
            fsm->pos   = 0;
            fsm->state = STATE_W;
        }
        break;

    case STATE_W:
        ret = write(fsm->dfd, (fsm->buf + fsm->pos), fsm->len);
        if (ret < 0) {
            if (errno == EAGAIN) {
                fsm->state = STATE_W;
            }
            else {
                fsm->errstr = "write failed";
                fsm->state  = STATE_E;
            }
        }
        else {
            fsm->pos += ret;
            fsm->len -= ret;
            if (fsm->len == 0)
                fsm->state = STATE_R;
            else
                fsm->state = STATE_W;
        }
        break;

    case STATE_E:
        perror(fsm->errstr);
        fsm->state = STATE_T;
        break;

    case STATE_T:
        // do sth
        break;

    default:
        abort();
        break;
    }
}

static void relay(int fd1, int fd2)
{
    int fd1_save = fcntl(fd1, F_GETFL);
    fcntl(fd1, F_SETFL, fd1_save | O_NONBLOCK);
    int fd2_save = fcntl(fd2, F_GETFL);
    fcntl(fd2, F_SETFL, fd2_save | O_NONBLOCK);

    // 读左写右,读右写左
    fsm_t fsm_rl_wr = { .state = STATE_R, .sfd = fd1, .dfd = fd2 };
    fsm_t fsm_rr_wl = { .state = STATE_R, .sfd = fd2, .dfd = fd1 };

    struct pollfd pfd[2];
    pfd[0].fd = fd1;
    pfd[1].fd = fd2;

    while (fsm_rl_wr.state != STATE_T || fsm_rr_wl.state != STATE_T) {
        // 设置监视任务
        pfd[0].events = 0; // 每次将事件置为0
        if (fsm_rl_wr.state == STATE_R)
            pfd[0].events |= POLLIN;
        if (fsm_rl_wr.state == STATE_W)
            pfd[0].events |= POLLOUT;

        pfd[1].events = 0;
        if (fsm_rr_wl.state == STATE_R)
            pfd[1].events |= POLLIN;
        if (fsm_rr_wl.state == STATE_W)
            pfd[1].events |= POLLOUT;

        // 监视
        int max = (fd1 > fd2) ? fd1 : fd2;
        // 仅当状态机的状态为读写时才监视
        if (fsm_rl_wr.state < STATE_E || fsm_rr_wl.state < STATE_E) {
            while (poll(pfd, 2, -1) < 0) {
                if (errno == EINTR)
                    continue;
                perror("poll failed");
                exit(EXIT_FAILURE);
            }
        }

        // 查看监视结果,1可读2可写或者为异常态和终止态就推状态机
        if (pfd[0].revents & POLLIN || pfd[0].revents & POLLOUT || fsm_rl_wr.state >= STATE_E) {
            fsm_driver(&fsm_rl_wr);
        }
        if (pfd[1].revents & POLLIN || pfd[1].revents & POLLOUT || fsm_rr_wl.state >= STATE_E) {
            fsm_driver(&fsm_rr_wl);
        }
    }

    // 模块化思想,进入该模块什么状态,出去就是什么状态
    fcntl(fd1, F_SETFL, fd1_save);
    fcntl(fd2, F_SETFL, fd2_save);
}

static int open_file(const char* path, int flag)
{
    int fd = open(path, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "%s open failed", path);
        exit(EXIT_FAILURE);
    }
    return fd;
};

int main(void)
{

    int fd1 = open_file(TTY1, O_RDWR);
    write(fd1, "tty1\n", 5);

    int fd2 = open_file(TTY2, O_RDWR | O_NONBLOCK);
    write(fd2, "tty2\n", 5);

    relay(fd1, fd2);

    return 0;
}
