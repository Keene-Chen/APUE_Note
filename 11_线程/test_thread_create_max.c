/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.19-21:11:34
 * Description: 测试单进程最大可创建多少线程
 * Command    : gcc test_thread_create_max.c -pthread
 * Environment: Linux x64 1H2G
 * Result     : 32752个 资源瓶颈不是栈大小,而是tid耗尽
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void* handler(void* arg)
{
    pthread_exit(NULL);
}

int main(void)
{
    int err, count;
    pthread_t tid;

    while (err == 0) {
        err = pthread_create(&tid, NULL, handler, NULL);
        count++;
    }
    fprintf(stderr, "create failed: %s %d", strerror(err), count);

    return 0;
}