/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.19-20:08:43
 * Description: 03_demo_prime 使用线程找质数
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEFT 30000000
#define RIGHT 30000200
#define THRMAX (RIGHT - LEFT + 1)

static void* thr_prime(void* arg)
{
    int i    = (int)arg;
    int mark = 1;

    for (int j = 2; j <= i / 2; j++) {
        if (i % j == 0) {
            mark = 0;
            break;
        }
    }

    if (mark)
        printf("%d is a primer\n", i);

    pthread_exit(NULL);
}

int main(void)
{
    pthread_t tid[THRMAX];

    int err;
    for (int i = LEFT; i <= RIGHT; i++) {
        err = pthread_create(tid + (i - LEFT), NULL, thr_prime, (void*)i); // 将i强制为地址,解决竞争问题
        if (err != 0) {
            fprintf(stderr, "pthread_create failed: %s", strerror(err));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = LEFT; i <= RIGHT; i++)
        pthread_join(tid[i - LEFT], NULL);

    return 0;
}