/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.18-21:13:28
 * Description: 08_sigsuspend_rt 实时信号不丢失
 * Command: kill -40 606386
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MYRTSIG (SIGRTMIN+6) 

const char* str1 = "*";
const char* str2 = "!";

void int_handler(int s)
{
    write(STDOUT_FILENO, str2, strlen(str2));
}

int main(void)
{
    // 注册信号
    signal(MYRTSIG, int_handler);

    // 设置信号集
    sigset_t set, oset, saveset;
    sigemptyset(&set);
    sigaddset(&set, MYRTSIG);

    sigprocmask(SIG_UNBLOCK, &set, &saveset);

    sigprocmask(SIG_BLOCK, &set, &oset);
    for (int i = 0; i < 100; i++) {
        for (int i = 0; i < 5; i++) {
            write(STDOUT_FILENO, str1, strlen(str1));
            sleep(1);
        }
        write(STDOUT_FILENO, "\n", 1);

        sigsuspend(&oset);
    }

    sigprocmask(SIG_SETMASK, &saveset, NULL);

    return 0;
}