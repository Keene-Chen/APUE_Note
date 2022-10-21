/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.20-13:44:52
 * Description: 04_pthread_mutex_xxx
 */

/**
 * 常用 API
 *
 * * int pthread_mutex_init(pthread_mutex_t *restrict mutex,const pthread_mutexattr_t *restrict attr);
 * * pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
 * @brief  互斥量初始化
 * @param  mutex pthread_mutex_t类型的互斥量变量
 * @param  attr 属性
 * @return 成功返回0,失败返回错误码
 *
 * * int pthread_mutex_destroy(pthread_mutex_t *mutex);
 * @brief  互斥量销毁
 * @param  mutex pthread_mutex_t类型的互斥量变量
 * @return 成功返回0,失败返回错误码
 *
 * * int pthread_mutex_lock(pthread_mutex_t *mutex);
 * * int pthread_mutex_trylock(pthread_mutex_t *mutex);
 * * int pthread_mutex_unlock(pthread_mutex_t *mutex);
 * @brief  互斥量加锁、尝试加锁、解锁
 * @param  mutex pthread_mutex_t类型的互斥量变量
 * @return 成功返回0,失败返回错误码
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define THRMAX 20
#define LINESIZE 1024
#define FILENAME "/tmp/out"

static pthread_mutex_t mutex;

static void* thr_add(void* arg)
{
    char linebuf[LINESIZE];

    FILE* fp = fopen(FILENAME, "r+");
    if (fp == NULL) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&mutex); // 加锁
    fgets(linebuf, LINESIZE, fp);
    // sleep(1); // 放大竞争现象
    fseek(fp, 0, SEEK_SET);
    fprintf(fp, "%d\n", atoi(linebuf) + 1);
    fclose(fp);
    pthread_mutex_unlock(&mutex); // 解锁

    pthread_exit(NULL);
}

int main(void)
{
    pthread_t tid[THRMAX];
    int err;

    // 初始化互斥量
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < THRMAX; i++) {
        err = pthread_create(tid + i, NULL, thr_add, NULL);
        if (err != 0) {
            fprintf(stderr, "pthread_create failed: %s", strerror(err));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < THRMAX; i++) {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}