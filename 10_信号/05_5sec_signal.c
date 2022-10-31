/**
 * @file    : 05_5sec_signal.c
 * @author  : KeeneChen
 * @date    : 2022.10.31-21:56:04
 * @details : 信号实现版本
 * @command : gcc 05_5sec_signal.c && time -p ./a.out
 * count: 2164837222
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int flag = 1;
static void alarm_handler(int s)
{
    flag = 0;
}

int main(void)
{
    int64_t count = 0;
    signal(SIGALRM, alarm_handler);
    alarm(5); // 由于alarm默认行为会终止进程（异常终止）导致无法输出，所以使用信号

    while (flag)
        count++;
    printf("%ld\n", count);

    return 0;
}