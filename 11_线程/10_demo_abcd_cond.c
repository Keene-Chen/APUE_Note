/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.21-17:07:27
 * Description: 10_demo_abcd_cond
 * ! Warning  : 该实现有强制转换警告只是用于理解互斥量,互斥量锁住的是整段代码而不是具体的哪一个变量或函数
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>
#include <unistd.h>

#define THRMAX 4
static pthread_mutex_t mutex[THRMAX];

static int next(int n)
{
    if ((n + 1) == THRMAX)
        return 0;
    return n + 1;
}

static void* thr_func(void* arg)
{
    int n  = (int)arg;
    int ch = 'a' + n; // arg = 0,1,2,3

    while (1) {
        pthread_mutex_lock(mutex + n); // 锁自己,等待下一个线程来解锁
        write(STDOUT_FILENO, &ch, sizeof(ch));
        pthread_mutex_unlock(mutex + next(n)); // 解锁下一个线程的互斥量
    }

    pthread_exit(NULL);
}

int main(void)
{
    pthread_t tid[THRMAX];
    int err;

    for (int i = 0; i < THRMAX; i++) {
        pthread_mutex_init(&mutex[i], NULL);
        pthread_mutex_lock(&mutex[i]);

        err = pthread_create(&tid[i], NULL, thr_func, (void*)i);
        if (err != 0) {
            fprintf(stderr, "pthread_create failed: %s", strerror(err));
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_unlock(mutex + 0); // 解锁第一个线程

    alarm(3); // 由于线程中执行的是死循环执行不到线程回收,所以使用信号杀死当前进程

    // ! warning: 程序是运行不到这里的
    for (int i = 0; i < THRMAX; i++) {
        pthread_join(tid[i], NULL);
        pthread_mutex_destroy(&mutex[i]);
    }

    return 0;
}