/**
 * @file    : mysem_test.c
 * @author  : KeeneChen
 * @date    : 2022.10.17-21:24:03
 * @details : mysem_test
 */

#include "mysem.h"

#define N 4
#define LEFT 30000000
#define RIGHT 30000200
#define THRMAX (RIGHT - LEFT + 1)

static mysem_t* sem;

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

    sleep(5);

    mysem_add(sem, 1);

    pthread_exit(NULL);
}

int main(void)
{
    pthread_t tid[THRMAX];
    int err;
    sem = mysem_init(N);
    if (sem == NULL) {
        fprintf(stderr, "mysem_init failed\n");
        exit(EXIT_FAILURE);
    }

    for (int i = LEFT; i <= RIGHT; i++) {
        mysem_sub(sem, 1);
        err = pthread_create(tid + (i - LEFT), NULL, thr_prime, (void*)i); // 将i强制为地址,解决竞争问题
        if (err != 0) {
            fprintf(stderr, "pthread_create failed: %s", strerror(err));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = LEFT; i <= RIGHT; i++)
        pthread_join(tid[i - LEFT], NULL);
    mysem_destory(sem);

    return 0;
}