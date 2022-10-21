/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.17-10:21:46
 * Description: volatile 优化版
 * Command: gcc 03_5sec_volatile.c -O1 && time -p ./a.out
 * time:     1341882153
 * signal:   2162453388
 * volatile: 11146237004
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static volatile int flag = 1;
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