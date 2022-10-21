/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.17-21:24:03
 * Description: mytbf
 */

#ifndef __MYTBF_H__
#define __MYTBF_H__

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

/* 令牌最大值 */
#define MYTBF_MAX 1024

typedef void mytbf_t;

/**
 * @brief  初始化令牌桶
 * @param  cps 流控大小
 * @param  burst 流控上限
 * @return mytbf_t* 令牌桶指针
 */
mytbf_t* mytbf_init(int cps, int burst);

/**
 * @brief  销毁令牌桶
 * @param  ptr 令牌桶指针
 * @return int 成功返回0,永不失败
 */
int mytbf_destroy(mytbf_t* ptr);

/**
 * @brief  获取令牌
 * @param  ptr 令牌桶指针
 * @param  size 申请令牌数量
 * @return int 成功返回申请的令牌数量,如果申请数量大于最大令牌计数则返回最大令牌计数,失败返回-EINVAL
 */
int mytbf_fetch_token(mytbf_t* ptr, int size);

/**
 * @brief  归还令牌
 * @param  ptr 令牌桶指针
 * @param  size 申请令牌数量
 * @return int 成功返回归还的令牌数量,失败返回-EINVAL
 */
int mytbf_return_token(mytbf_t* ptr, int size);

#endif // __MYTBF_H__