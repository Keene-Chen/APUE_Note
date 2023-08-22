/**
 * @file    : 01_signal.c
 * @author  : KeeneChen
 * @date    : 2022.10.14-13:50:31
 * @details : signal注册信号
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handler(int s)
{
    write(1, "!", 1);
}

int main(void)
{
    // signal(SIGINT, SIG_IGN);
    signal(SIGINT, handler);

    for (int i = 0; i < 10; i++) {
        write(1, "*", 1);
        sleep(1);
    }

    return 0;
}