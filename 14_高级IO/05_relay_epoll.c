/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.27-08:11:45
 * Description: 05_relay_epoll 使用epoll重构relay
 */

/**
 * IO多路复用-epoll相关API
 *
 * * int epoll_create(int size);
 * @brief 创建一个树根
 * @param size 最大节点数, 此参数在linux 2.6.8已被忽略, 但必须传递一个大于0的数
 * @return 成功: 返回一个大于0的文件描述符, 代表整个树的树根,失败: 返回-1, 并设置errno值
 *
 * * int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
 * @brief 将要监听的节点在epoll树上添加, 删除和修改
 * @param epfd epoll树根
 * @param op
 *             EPOLL_CTL_ADD: 添加事件节点到树上
 *             EPOLL_CTL_DEL: 从树上删除事件节点
 *             EPOLL_CTL_MOD: 修改树上对应的事件节点
 * @param fd   事件节点对应的文件描述符
 * @param event 要操作的事件节点
 *
 * * int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
 * @brief 等待内核返回事件发生
 * @param epfd      epoll树根
 * @param events    传出参数, 其实是一个事件结构体数组
 * @param maxevents 数组大小
 * @param timeout
 *                  -1: 表示永久阻塞
 *                   0: 立即返回
 *                  >0: 表示超时等待事件
 * @return 成功: 返回发生事件的个数,失败: 若timeout=0,没有事件发生则返回;返回-1,设置errno值
 *
 * * 保存触发事件的某个文件描述符相关的数据（与具体使用方式有关）
 * * typedef union epoll_data {
 * *     void *ptr;
 * *     int fd;
 * *     __uint32_t u32;
 * *     __uint64_t u64;
 * * } epoll_data_t;
 *
 * * 感兴趣的事件和被触发的事件
 * * struct epoll_event {
 * *     __uint32_t events;
 * *     epoll_data_t data;
 * * };
 *
 * events事件
 * EPOLLIN : 表示对应的文件描述符可以读（包括对端SOCKET正常关闭）;
 * EPOLLOUT: 表示对应的文件描述符可以写;
 * EPOLLPRI: 表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）;
 * EPOLLERR: 表示对应的文件描述符发生错误;
 * EPOLLHUP: 表示对应的文件描述符被挂断;
 * EPOLLET : 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
 * EPOLLONESHOT: 只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里
 *
 * 优点:
 * ? 1.支持一个进程打开大数目的socket描述符(FD)
 * ? 2.IO效率不随FD数目增加而线性下降
 * ? 3.使用mmap加速内核与用户空间的消息传递
 * 缺点:
 * ? 1.是Linux本地方言,不能跨平台
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
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

    // 添加监测文件描述符
    struct epoll_event ev;
    int epfd = epoll_create(10);
    if (epfd < 0) {
        perror("epoll_create failed");
        exit(EXIT_FAILURE);
    }

    ev.events  = 0;
    ev.data.fd = fd1;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd1, &ev);
    ev.events  = 0;
    ev.data.fd = fd2;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd2, &ev);

    while (fsm_rl_wr.state != STATE_T || fsm_rr_wl.state != STATE_T) {
        // 设置监视任务
        ev.events  = 0;
        ev.data.fd = fd1;
        if (fsm_rl_wr.state == STATE_R)
            ev.events |= EPOLLIN;
        if (fsm_rl_wr.state == STATE_W)
            ev.events |= EPOLLOUT;
        epoll_ctl(epfd, EPOLL_CTL_MOD, fd1, &ev);

        ev.events  = 0;
        ev.data.fd = fd2;
        if (fsm_rr_wl.state == STATE_R)
            ev.events |= EPOLLIN;
        if (fsm_rr_wl.state == STATE_W)
            ev.events |= EPOLLOUT;
        epoll_ctl(epfd, EPOLL_CTL_MOD, fd2, &ev);

        // 监视
        int max = (fd1 > fd2) ? fd1 : fd2;
        // 仅当状态机的状态为读写时才监视
        if (fsm_rl_wr.state < STATE_E || fsm_rr_wl.state < STATE_E) {
            while (epoll_wait(epfd, &ev, 1, -1) < 0) {
                if (errno == EINTR)
                    continue;
                perror("epoll_wait failed");
                exit(EXIT_FAILURE);
            }
        }

        // 查看监视结果,1可读2可写或者为异常态和终止态就推状态机
        if (ev.data.fd == fd1 && ev.events & EPOLLIN || ev.data.fd == fd2 && ev.events & EPOLLOUT || fsm_rl_wr.state >= STATE_E) {
            fsm_driver(&fsm_rl_wr);
        }
        if (ev.data.fd == fd2 && ev.events & EPOLLIN || ev.data.fd == fd1 && ev.events & EPOLLOUT || fsm_rr_wl.state >= STATE_E) {
            fsm_driver(&fsm_rr_wl);
        }
    }

    // 模块化思想,进入该模块什么状态,出去就是什么状态
    fcntl(fd1, F_SETFL, fd1_save);
    fcntl(fd2, F_SETFL, fd2_save);

    // ! 执行不到这
    close(epfd);
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
