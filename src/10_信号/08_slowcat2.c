/**
 * @file    : 08_slowcat2.c
 * @author  : KeeneChen
 * @date    : 2022.10.17-14:58:45
 * @details : 流控cat实现(令牌桶)
 */

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 10
#define BURST 100 // 最大权限值

static volatile int token = 0;
static void alrm_handler(int s)
{
    alarm(1);
    token++;
    if (token > BURST)
        token = BURST;
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
    alarm(1);

    // 读写操作
    while (1) {
        while (token <= 0)
            pause(); // 令牌桶等待，累加权限值token
        token--;

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