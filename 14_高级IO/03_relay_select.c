/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.25-16:52:57
 * Description: 03_relay_select 使用select重构relay
 */

/**
 * IO多路复用-select相关API
 *
 * * int select(int nfds, fd_set *readfds, fd_set *writefds,
 * *            fd_set *exceptfds, struct timeval *timeout);
 * @brief 允许程序监视多个文件描述符,等待所监视的一个或者多个文件描述符变为可读、可写、异常三种状态时返回结果
 * @param nfds      表示集合中所有文件描述符的范围,即所有文件描述符的最大值+1
 * @param readfds   监视读文件描述符集合
 * @param writefds  监视写文件描述符集合
 * @param exceptfds 监视异常文件描述符集合
 * @param timeout   超时设置
 *
 * * void FD_ZERO(fd_set *fdset);        //清空fdset中所有文件描述符
 * * void FD_SET(int fd,fd_set *fdset);  //添加文件描述符fd到集合fdset中
 * * void FD_CLR(int fd,fd_set *fdset);  //将文件描述符fd从集合fdset中去除
 * * int FD_ISSET(int fd,fd_set *fdset); //判断文件描述符fd是否在集合fdset中
 *
 * 优点:
 * ? 1.select的可移植性较好,可以跨平台；
 * ? 2.select可设置的监听时间timeout精度更好,可精确到微秒,而poll为毫秒
 * 缺点:
 * ? 1.select支持的文件描述符数量上限为1024,不能根据用户需求进行更改；
 * ? 2.select每次调用时都要将文件描述符集合从用户态拷贝到内核态,开销较大；
 * ? 3.select返回的就绪文件描述符集合,需要用户循环遍历所监听的所有文件描述符是否在该集合中,当监听描述符数量很大时效率较低。
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#define TTY1 "/dev/pts/1"
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

    fd_set rset, wset;

    while (fsm_rl_wr.state != STATE_T || fsm_rr_wl.state != STATE_T) {
        // 设置监视任务
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        if (fsm_rl_wr.state == STATE_R)
            FD_SET(fsm_rl_wr.sfd, &rset);
        else if (fsm_rl_wr.state == STATE_W)
            FD_SET(fsm_rl_wr.dfd, &wset);
        if (fsm_rr_wl.state == STATE_R)
            FD_SET(fsm_rr_wl.sfd, &rset);
        else if (fsm_rr_wl.state == STATE_W)
            FD_SET(fsm_rr_wl.dfd, &wset);

        // 监视
        int max = (fd1 > fd2) ? fd1 : fd2;
        // 仅当状态机的状态为读写时才监视
        if (fsm_rl_wr.state < STATE_E || fsm_rr_wl.state < STATE_E) {
            if (select(max + 1, &rset, &wset, NULL, NULL) < 0) {
                if (errno == EINTR)
                    continue;
                perror("select failed");
                exit(EXIT_FAILURE);
            }
        }

        // 查看监视结果,1可读2可写或者为异常态和终止态就推状态机
        if (FD_ISSET(fd1, &rset) || FD_ISSET(fd2, &wset) || fsm_rl_wr.state >= STATE_E) {
            fsm_driver(&fsm_rl_wr);
        }
        if (FD_ISSET(fd2, &rset) || FD_ISSET(fd1, &wset) || fsm_rr_wl.state >= STATE_E) {
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
