/**
 * @file    : 02_pthread_cleanup_push.c
 * @author  : KeeneChen
 * @date    : 2022.10.19-16:53:51
 * @details : 02_pthread_cleanup_push
 */

/**
 * 线程清理相关API
 *
 * * void pthread_cleanup_push(void (*routine)(void *),void *arg);
 * @brief  在当前线程注册一个清理回调函数
 * @param  routine 注册的回调函数
 * @param  arg     注册的回调函数的参数
 *
 * * void pthread_cleanup_pop(int execute);
 * @brief  删除线程栈顶上的清理回调函数
 * @param  execute 非零才调用
 * @note   该函数是宏定义函数与上述函数类似
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_func(void* arg)
{
    puts(arg);
}

static void* func(void* arg)
{
    printf("thread is working\n");

    pthread_cleanup_push(cleanup_func, "cleanup_1");
    pthread_cleanup_push(cleanup_func, "cleanup_2");
    pthread_cleanup_push(cleanup_func, "cleanup_3");

    puts("push over");

    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(0);

    pthread_exit(NULL);
}

int main(void)
{
    pthread_t tid;

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