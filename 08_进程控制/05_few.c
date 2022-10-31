/**
 * @file    : 05_few.c
 * @author  : KeeneChen
 * @date    : 2022.10.12-18:58:45
 * @details : 联用 fork execl wait函数
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    printf("begin\n");

    fflush(NULL);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        execl("/bin/date", "date", "+%s", NULL);
        perror("execl failed");
        exit(EXIT_FAILURE);
    }

    wait(NULL);
    printf("end\n");

    return 0;
}