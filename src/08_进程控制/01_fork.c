/**
 * @file    : 01_fork.c
 * @author  : KeeneChen
 * @date    : 2022.10.12-18:56:53
 * @details : fork创建子进程
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    printf("parent:%d child:%d\n", getppid(), getpid());

    fflush(NULL); // 在fork之前应该刷新缓冲区

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        printf("The child process is running\n");
        printf("parent:%d child:%d\n", getppid(), getpid());
    }
    else {
        printf("The parent process is running\n");
        printf("parent:%d child:%d\n", getppid(), getpid());
        sleep(1);
    }

    return 0;
}