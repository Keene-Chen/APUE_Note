/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.20-15:20:32
 * Description: 08_demo_prime_pool 多线程找质数,使用任务池实现
 */

#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEFT 30000000
#define RIGHT 30000200
#define THRMAX 4 // 线程最大数

static int task_num          = 0; // 使用全局变量,模拟任务池中的任务
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void* thr_prime(void* arg)
{
    while (1) {
        pthread_mutex_lock(&mutex);
        while (task_num == 0) {
            pthread_mutex_unlock(&mutex);
            sched_yield();
            pthread_mutex_lock(&mutex);
        }

        if (task_num == -1) {
            pthread_mutex_unlock(&mutex); // 跳出代码临界区时需要先解锁
            break;
        }

        int i    = task_num;
        task_num = 0; // 等待任务下发
        pthread_mutex_unlock(&mutex);

        int mark = 1;
        for (int j = 2; j <= i / 2; j++) {
            if (i % j == 0) {
                mark = 0;
                break;
            }
        }
        if (mark)
            printf("[%d]: %d is a primer\n", (int)arg, i);
    }

    pthread_exit(NULL);
}

int main(void)
{
    pthread_t tid[THRMAX];
    int err;

    for (int i = 0; i < THRMAX; i++) {
        err = pthread_create(&tid[i], NULL, thr_prime, (void*)i); // 将i强制为地址,解决竞争问题
        if (err != 0) {
            fprintf(stderr, "pthread_create failed: %s", strerror(err));
            exit(EXIT_FAILURE);
        }
    }

    /**
     * 任务池实现
     * task_num > 0 下发任务
     *          = 0 等待任务下发
     *          = -1 退出
     */
    for (int i = LEFT; i <= RIGHT; i++) {
        pthread_mutex_lock(&mutex);
        while (task_num != 0) {
            pthread_mutex_unlock(&mutex);
            sched_yield(); // 调度器挂起,理解为短暂的sleep,且线程状态不会发生切换
            pthread_mutex_lock(&mutex);
        }
        task_num = i;
        pthread_mutex_unlock(&mutex);
    }

    // 修改全局共享变量时,需要先加锁
    pthread_mutex_lock(&mutex);
    while (task_num != 0) {
        pthread_mutex_unlock(&mutex);
        sched_yield();
        pthread_mutex_lock(&mutex);
    }
    task_num = -1;
    pthread_mutex_unlock(&mutex);

    // 等待线程回收,释放互斥量
    for (int i = 0; i < THRMAX; i++) {
        pthread_join(tid[i], NULL);
    }
    pthread_mutex_destroy(&mutex);

    return 0;
}