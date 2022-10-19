/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.18-16:38:41
 * Description: 06_sigsuspend
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char* str1 = "*";
const char* str2 = "!";

void int_handler(int s)
{
    write(STDOUT_FILENO, str2, strlen(str2));
}

int main(void)
{
    // 注册信号
    signal(SIGINT, int_handler);

    // 设置信号集
    sigset_t set, oset, saveset;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);

    sigprocmask(SIG_UNBLOCK, &set, &saveset);

    sigprocmask(SIG_BLOCK, &set, &oset);
    for (int i = 0; i < 100; i++) {
        for (int i = 0; i < 5; i++) {
            write(STDOUT_FILENO, str1, strlen(str1));
            sleep(1);
        }
        write(STDOUT_FILENO, "\n", 1);

        /**
         * sigset_t tmpset;
         * sigprocmask(SIG_SETMASK, &oset, &tmpset);
         * pause();
         * sigprocmask(SIG_SETMASK, &tmpset, NULL);
         * sigsuspend相当于上面四句的原子操作
         */
        sigsuspend(&oset);
    }

    sigprocmask(SIG_SETMASK, &saveset, NULL);

    return 0;
}