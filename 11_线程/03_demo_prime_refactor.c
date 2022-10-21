/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.19-20:08:43
 * Description: 03_demo_prime
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEFT 30000000
#define RIGHT 30000200
#define THRMAX (RIGHT - LEFT + 1)

struct thr_arg_t {
    int n;
};

static void* thr_prime(void* arg)
{
    int i    = ((struct thr_arg_t*)arg)->n;
    int mark = 1;

    for (int j = 2; j <= i / 2; j++) {
        if (i % j == 0) {
            mark = 0;
            break;
        }
    }

    if (mark)
        printf("%d is a primer\n", i);

    pthread_exit(arg);
}

int main(void)
{
    pthread_t tid[THRMAX];
    struct thr_arg_t* thr_arg_ptr;
    void* ptr; // 接受线程返回的值
    int err;

    for (int i = LEFT; i <= RIGHT; i++) {
        thr_arg_ptr = malloc(sizeof(*thr_arg_ptr));
        if (thr_arg_ptr == NULL) {
            perror("malloc failed");
            exit(EXIT_FAILURE);
        }
        thr_arg_ptr->n = i;

        err = pthread_create(tid + (i - LEFT), NULL, thr_prime, thr_arg_ptr); // 传递结构体指针,解决强制转换问题
        if (err != 0) {
            fprintf(stderr, "pthread_create failed: %s", strerror(err));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = LEFT; i <= RIGHT; i++) {
        pthread_join(tid[i - LEFT], &ptr);
        free(ptr); // 释放线程返回回来的指针(传入的malloc结构体)
    }

    return 0;
}