/**
 * @file    : 09_pthread_cond_xxx.c
 * @author  : KeeneChen
 * @date    : 2022.10.21-16:02:54
 * @details : 使用多线程中的条件变量找质数,非忙等
 */

/**
 * 条件变量相关API
 *
 * 定义一个条件变量
 * * pthread_cond_t  cond;
 *
 * * int pthread_cond_init(pthread_cond_t *restrict cond,
 * *                       const pthread_condattr_t *restrict attr);
 * @brief  初始化条件变量
 * @param  cond 条件变量的地址
 * @param  attr 条件变量属性的地址
 * @return int  成功返回0,失败返回错误号
 *
 * * int pthread_cond_destroy(pthread_cond_t *cond);
 * @brief  销毁条件变量
 * @param  cond 条件变量的地址
 * @return int  成功返回0,失败返回错误号
 *
 * * int pthread_cond_wait(pthread_cond_t *restrict cond,
 * *                       pthread_mutex_t *restrict mutex);
 * @brief  条件不满足,引起线程阻塞并解锁;条件满足,解除线程阻塞,并加锁
 * @param  cond  条件变量的地址
 * @param  mutex 互斥量的地址
 * @return int   成功返回0,失败返回错误号
 *
 * * int pthread_cond_broadcast(pthread_cond_t *cond);
 * @brief  唤醒所有阻塞在该条件变量上的线程
 * @param  cond 条件变量的地址
 * @return int  成功返回0,失败返回错误号
 *
 * * int pthread_cond_signal(pthread_cond_t *cond);
 * @brief  唤醒至少一个阻塞在该条件变量上的线程
 * @param  cond 条件变量的地址
 * @return int  成功返回0,失败返回错误号
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
static pthread_cond_t cond   = PTHREAD_COND_INITIALIZER;

static void* thr_prime(void* arg)
{
    while (1) {
        pthread_mutex_lock(&mutex);
        while (task_num == 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        if (task_num == -1) {
            pthread_mutex_unlock(&mutex); // 跳出代码临界区时需要先解锁
            break;
        }

        int i    = task_num;
        task_num = 0;                  // 等待任务下发
        pthread_cond_broadcast(&cond); // 通知上游线程下发任务
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
            pthread_cond_wait(&cond, &mutex);
        }
        task_num = i;
        pthread_cond_signal(&cond); // 唤醒下游任何一个线程执行任务
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
    pthread_cond_broadcast(&cond); // 唤醒下游所有线程退出
    pthread_mutex_unlock(&mutex);

    // 等待线程回收,释放互斥量
    for (int i = 0; i < THRMAX; i++) {
        pthread_join(tid[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}