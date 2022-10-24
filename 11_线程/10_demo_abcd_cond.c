/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.23-21:56:31
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
static int num               = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond   = PTHREAD_COND_INITIALIZER;

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
        pthread_mutex_lock(&mutex);
        while (num != n) {
            pthread_cond_wait(&cond, &mutex);
        }
        write(STDOUT_FILENO, &ch, sizeof(ch));
        num = next(num);
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main(void)
{
    pthread_t tid[THRMAX];
    int err;

    for (int i = 0; i < THRMAX; i++) {
        err = pthread_create(&tid[i], NULL, thr_func, (void*)i);
        if (err != 0) {
            fprintf(stderr, "pthread_create failed: %s", strerror(err));
            exit(EXIT_FAILURE);
        }
    }

    alarm(3); // 由于线程中执行的是死循环执行不到线程回收,所以使用信号杀死当前进程

    // ! warning: 程序是运行不到这里的
    for (int i = 0; i < THRMAX; i++) {
        pthread_join(tid[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}