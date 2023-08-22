/**
 * @file    : 01_pipe.c
 * @author  : KeeneChen
 * @date    : 2022.10.27-16:25:54
 * @details : 匿名管道只能用于有亲缘关系的进程间通信
 */

/**
 * 管道相关API
 *
 * * int pipe(int fd[2]);
 * @brief 创建一个管道，以实现进程间的通信
 * @param fd,经参数fd返回的两个文件描述符,fd[0]为读端,fd[1]为写端,fd[1]的输出是fd[0]的输入
 * @return 函数是否执行成功
 * @retval 成功: 0
 * @retval 出错: -1并设置errno
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(void)
{
    int fd[2];
    char buf[BUF_SIZE];

    int ret = pipe(fd);
    if (ret < 0) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {                      // child read
        close(fd[1]);                         // 子进程关闭写端
        int len = read(fd[0], buf, BUF_SIZE); // 读管道
        write(STDOUT_FILENO, buf, len);       // 写到标准输出
        close(fd[0]);                         // 子进程读完关闭读端
        exit(EXIT_SUCCESS);
    }
    else {                                  // parent write
        close(fd[0]);                       // 父进程关闭读端
        write(STDOUT_FILENO, "hello\n", 6); // 写管道
        close(fd[1]);                       // 父进程写完关闭写端
        wait(NULL);
        exit(EXIT_SUCCESS);
    }

    return 0;
}