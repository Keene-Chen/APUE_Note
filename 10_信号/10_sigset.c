/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.18-15:56:48
 * Description: 10_sigset
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

    /**
     * ! 保证程序模块化的健壮性,
     * 开始取消任何阻塞信号并保存进入该模块前的状态,
     * 最后离开模块后恢复之前的状态。
     * ! 不要在信号处理函数中使用longjmp函数,因为会丢失之前的状态,
     * 造成今后在该模块内无法接收到信号。
     */
    sigprocmask(SIG_UNBLOCK, &set, &saveset);

    for (int i = 0; i < 100; i++) {
        sigprocmask(SIG_BLOCK, &set, &oset);
        for (int i = 0; i < 5; i++) {
            write(STDOUT_FILENO, str1, strlen(str1));
            sleep(1);
        }
        write(STDOUT_FILENO, "\n", 1);
        sigprocmask(SIG_SETMASK, &oset, NULL);
    }
    sigprocmask(SIG_SETMASK, &saveset, NULL);

    return 0;
}