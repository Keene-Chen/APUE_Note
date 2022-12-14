/**
 * @file    : 03_waitpid.c
 * @author  : KeeneChen
 * @date    : 2022.10.12-19:00:53
 * @details : waitpid回收指定子进程
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    printf("[%d]:begin\n", getpid());

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        printf("[%d]:child is runing\n", getpid());
    }
    else if (pid > 0) {
        printf("[%d]:parent is runing\n", getpid());

        int status;
        pid_t wpid = waitpid(-1, &status, WNOHANG);
        printf("%d\n", wpid);
    }

    return 0;
}