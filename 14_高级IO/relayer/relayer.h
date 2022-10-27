/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.25-16:11:20
 * Description: relayer
 */

#ifndef __RELAYER_H__
#define __RELAYER_H__

#include "mimalloc-2.0/mimalloc.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/* 最大任务数 */
#define REL_JOB_MAX 10000

/* 任务状态 */
enum { STATE_RUNNING = 1, STATE_CANCELD, STATE_OVER };

/* 任务状态结构体 */
typedef struct rel_stat_s {
    int state;
    int fd[3];
    int64_t count12, count21;
} rel_stat_t;

/**
 * @brief  添加任务
 * @param  fd 一对文件描述符
 * @return >= 0 get job id
 *         == -EINVAL 参数非法
 *         == -ENOSPC 任务数组满
 *         == -ENOMEM 内存分配出错
 */
int rel_job_add(int fd1,int fd2);

/**
 * @brief  取消任务
 * @param  id 任务id
 * @return == 0 取消成功
 *         == -EINVAL 参数非法
 *         == -EBUSY  任务取消过了
 */
int rel_job_cancel(int id);

/**
 * @brief  等待任务
 * @param  id 任务id
 * @return == 0 返回状态
 *         == -EINVAL 参数非法
 */
int rel_job_wait(int id);

/**
 * @brief  任务状态
 * @param  id 任务id
 * @return == 0 指定任务状态已返回
 */
int rel_job_stat(int id, rel_stat_t*);

#endif // __RELAYER_H__