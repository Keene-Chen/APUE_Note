/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.24-19:31:27
 * Description: 01_relay 有限状态机实现
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
            else { // 真错写入错误消息，并进入异常态
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

    while (fsm_rl_wr.state != STATE_T || fsm_rr_wl.state != STATE_T) {
        fsm_driver(&fsm_rl_wr);
        fsm_driver(&fsm_rr_wl);
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
