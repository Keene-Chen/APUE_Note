/**
 * @file   : mypipe.c
 * @author : KeeneChen
 * @date   : 2022.10.28-10:31:56
 */

#include "mypipe.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct mypipe_s {
    int head;
    int tail;
    char data[PIPE_SIZE];
    int data_size;
    int rd_cnt; // 读者计数
    int wr_cnt; // 写者计数
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} mypipe_st;

mypipe_t* mypipe_init(void)
{
    mypipe_st* pipe = (mypipe_st*)malloc(sizeof(mypipe_st));
    if (pipe == NULL)
        return NULL;
    pipe->head      = 0;
    pipe->tail      = 0;
    pipe->data_size = 0;
    pipe->rd_cnt    = 0;
    pipe->wr_cnt    = 0;
    pthread_mutex_init(&pipe->mutex, NULL);
    pthread_cond_init(&pipe->cond, NULL);

    return pipe;
}

int mypipe_destroy(mypipe_t* pipe)
{
    mypipe_st* ptr = pipe;
    pthread_mutex_destroy(&ptr->mutex);
    pthread_cond_destroy(&ptr->cond);
    free(ptr);

    return 0;
}

int mypipe_register(mypipe_t* pipe, int op_map)
{
    // if (op_map != PIPE_READ || op_map != PIPE_WRITE) {
    //     perror("mypipe_register failed");
    //     exit(EXIT_FAILURE);
    // }

    mypipe_st* ptr = pipe;
    pthread_mutex_lock(&ptr->mutex);
    if (op_map & PIPE_READ)
        ptr->rd_cnt++;
    if (op_map & PIPE_WRITE)
        ptr->wr_cnt++;

    // 当读写方任意一个不存在时,就等待
    while (ptr->rd_cnt <= 0 || ptr->wr_cnt <= 0)
        pthread_cond_wait(&ptr->cond, &ptr->mutex);

    // 当读写方都存在时就唤醒
    pthread_cond_broadcast(&ptr->cond);
    pthread_mutex_unlock(&ptr->mutex);

    return 0;
}

int mypipe_unregister(mypipe_t* pipe, int op_map)
{
    // if (op_map != PIPE_READ || op_map != PIPE_WRITE) {
    //     perror("mypipe_unregister failed");
    //     exit(EXIT_FAILURE);
    // }

    mypipe_st* ptr = pipe;
    pthread_mutex_lock(&ptr->mutex);
    if (op_map & PIPE_READ)
        ptr->rd_cnt--;
    if (op_map & PIPE_WRITE)
        ptr->wr_cnt--;

    // 当读写方都存在时就唤醒
    pthread_cond_broadcast(&ptr->cond);
    pthread_mutex_unlock(&ptr->mutex);

    return 0;
}

static int mypipe_readbyte_unlocked(mypipe_st* ptr, char* data)
{
    if (ptr->data_size <= 0)
        return -1;

    *data     = ptr->data[ptr->head];
    ptr->head = ((ptr->head + 1) == PIPE_SIZE) ? 0 : (ptr->head + 1);
    ptr->data_size--;

    return 0;
}

int mypipe_read(mypipe_t* pipe, void* buf, size_t count)
{
    mypipe_st* ptr = pipe;
    int i;

    pthread_mutex_lock(&ptr->mutex);
    // 当管道无数据和有写者时才等待
    while (ptr->data_size <= 0 && ptr->wr_cnt > 0)
        pthread_cond_wait(&ptr->cond, &ptr->mutex);

    // 如果管道无数据和没有写者时就返回
    if (ptr->data_size <= 0 && ptr->wr_cnt <= 0) {
        pthread_mutex_unlock(&ptr->mutex);
        return 0;
    }

    for (i = 0; i < count; i++) {
        if (mypipe_readbyte_unlocked(ptr, buf + i) != 0)
            break;
    }
    pthread_cond_broadcast(&ptr->cond);
    pthread_mutex_unlock(&ptr->mutex);

    return i;
}

static int mypipe_writebyte_unlocked(mypipe_st* ptr, const char* data)
{
    // 管道数据满
    if (ptr->data_size >= PIPE_SIZE) {
        return -1;
    }

    // 管道有数据 读取一个现在管道的读端数据,用data保存
    ptr->data[ptr->tail + 1] = *data;

    ptr->tail = (ptr->tail++) % PIPE_SIZE;
    ptr->data_size++;
    return 0;
}

int mypipe_write(mypipe_t* pipe, const void* buf, size_t size)
{
    mypipe_st* ptr = pipe;
    int i;

    pthread_mutex_lock(&ptr->mutex);

    while (ptr->data_size >= PIPE_SIZE && ptr->rd_cnt > 0) {
        pthread_cond_wait(&ptr->cond, &ptr->mutex);
    }

    // 管道空且没有读者
    if (ptr->data_size <= 0 && ptr->rd_cnt <= 0) {
        pthread_mutex_unlock(&ptr->mutex);
        return 0;
    }

    // 管道中有空间了
    for (i = 0; i < size; i++) {
        if (mypipe_writebyte_unlocked(ptr, buf + i) < 0) {
            break;
        }
    }
    pthread_mutex_unlock(&ptr->mutex);

    return i;
}
