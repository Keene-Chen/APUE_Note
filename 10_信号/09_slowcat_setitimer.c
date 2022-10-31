/**
 * @file    : 09_slowcat_setitimer.c
 * @author  : KeeneChen
 * @date    : 2022.10.18-15:23:53
 * @details : 使用setitimer实现
 */

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define BUF_SIZE 10

static volatile int flag = 0;
static void alrm_handler(int s)
{
    flag = 1;
}

int main(int argc, char const* argv[])
{
    int src_fd;                         // 读文件描述符
    int dest_fd        = STDOUT_FILENO; // 写文件描述符
    char buf[BUF_SIZE] = { 0 };         // 缓冲区
    int len = 0, ret = 0, pos;

    // 判断命令行参数个数
    if (argc < 2) {
        fprintf(stderr, "usage...\n");
        exit(EXIT_FAILURE);
    }

    // 保证文件打开成功
    do {
        src_fd = open(argv[1], O_RDONLY);
        if (src_fd < 0) {
            if (errno != EINTR) { // 防止阻塞的系统调用被中断
                fprintf(stderr, "open %s failed\n", argv[1]);
                exit(EXIT_FAILURE);
            }
        }
    } while (src_fd < 0);

    // 注册信号
    signal(SIGALRM, alrm_handler);

    // 设置时钟
    struct itimerval itv = { .it_interval.tv_sec = 1, .it_interval.tv_usec = 0, .it_value.tv_sec = 1, .it_value.tv_usec = 0 };
    if (setitimer(ITIMER_REAL, &itv, NULL) < 0) {
        perror("setitimer failed");
        exit(EXIT_FAILURE);
    }

    // 读写操作
    while (1) {
        while (!flag)
            pause(); // 漏桶等待，不做任何事
        flag = 0;

        while ((len = read(src_fd, buf, BUF_SIZE)) < 0) {
            if (errno == EINTR) // 如果系统调用被中断就又继续执行
                continue;
            else {
                perror("read failed");
                break;
            }
        }

        if (len == 0) {
            break;
        }

        pos = 0;
        while (len > 0) {
            ret = write(dest_fd, buf + pos, len);
            if (ret < 0) {
                if (errno == EINTR)
                    continue;
                else {
                    perror("write failed");
                    break;
                }
            }

            pos += ret;
            len -= ret;
        }

        // sleep(1); 由于sleep跨平台原因尽量不使用，这里才有信号实现流控的思路
    }

    close(src_fd);

    return 0;
}