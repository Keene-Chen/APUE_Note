/**
 * @file    : 04_execl.c
 * @author  : KeeneChen
 * @date    : 2022.10.12-18:51:59
 * @details : 04_execl
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
    execl("/bin/date", "date", "+%s", NULL);

    printf("end\n");

    return 0;
}