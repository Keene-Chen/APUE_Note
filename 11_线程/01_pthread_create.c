/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.19-16:03:57
 * Description: 01_pthread_create
 */

/**
 * 常用 API
 *
 * * int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);
 * @brief  创建一个新线程
 * @param  thread 线程标识
 * @param  attr   线程属性,为NULL是默认属性
 * @param  *start_routine   线程回调函数,运行起来就是新创建的线程
 * @param  arg 线程回调函数的参数
 * @return int 成功返回0,失败返回errornum
 * @note   编译和链接需要 -pthread 参数
 *
 * * void pthread_exit(void *retval);
 * @brief  线程退出
 * @param  retval 线程返回值 (pthread_join)
 *
 * * int pthread_join(pthread_t thread, void **retval);
 * @brief  等待一个线程结束
 * @param  retval 线程返回值地址 (pthread_exit)
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void* func(void* arg)
{
    printf("thread is working...\n");

    // return NULL;
    pthread_exit(NULL);
}

int main(void)
{
    pthread_t tid; // 线程标识类似pid

    puts(">>> begin >>>");

    int err = pthread_create(&tid, NULL, func, NULL);
    if (err != 0) {
        fprintf(stderr, "pthread_create failed: %s", strerror(err));
        exit(EXIT_FAILURE);
    }

    pthread_join(tid, NULL);

    puts("<<<  end  <<<");

    return 0;
}