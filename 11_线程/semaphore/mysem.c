#include "mysem.h"

typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} mysem_st;

mysem_t* mysem_init(int initval)
{
    mysem_st* sem = malloc(sizeof(mysem_st));
    if (sem == NULL)
        return NULL;

    sem->value = initval;
    pthread_mutex_init(&sem->mutex, NULL);
    pthread_cond_init(&sem->cond, NULL);

    return sem;
}

int mysem_add(mysem_t* ptr, int n)
{
    mysem_st* sem = ptr;

    pthread_mutex_lock(&sem->mutex);
    sem->value += n;
    pthread_cond_broadcast(&sem->cond);
    pthread_mutex_unlock(&sem->mutex);

    return n;
}

int mysem_sub(mysem_t* ptr, int n)
{
    mysem_st* sem = ptr;

    pthread_mutex_lock(&sem->mutex);
    while (sem->value < n)
        pthread_cond_wait(&sem->cond, &sem->mutex);
    sem->value -= n;
    pthread_mutex_unlock(&sem->mutex);

    return n;
}

int mysem_destory(mysem_t* ptr)
{
    mysem_st* sem = ptr;

    pthread_mutex_destroy(&sem->mutex);
    pthread_cond_destroy(&sem->cond);

    free(sem);
}