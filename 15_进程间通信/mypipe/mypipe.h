/**
 * @file   : mypipe.h
 * @author : KeeneChen
 * @date   : 2022.10.28-10:31:48
 */

#ifndef __MYPIPE_H__
#define __MYPIPE_H__

#include <stddef.h>

#define PIPE_SIZE 1024
#define PIPE_READ 0x00000001UL  // 读者位图
#define PIPE_WRITE 0x00000002UL // 写者

typedef void mypipe_t;

/**
 * @brief  管道初始化
 * @param  int
 * @param  int
 * @return int
 */
mypipe_t* mypipe_init(void);

/**
 * @brief  管道销毁
 * @param  int
 * @param  int
 * @return int
 */
int mypipe_destroy(mypipe_t* pipe);

/**
 * @brief  管道注册身份
 * @param  int
 * @param  int
 * @return int
 */
int mypipe_register(mypipe_t* pipe, int op_map);

/**
 * @brief  管道注销身份
 * @param  int
 * @param  int
 * @return int
 */
int mypipe_unregister(mypipe_t* pipe, int op_map);

/**
 * @brief  管道读
 * @param  int
 * @param  int
 * @return int
 */
int mypipe_read(mypipe_t* pipe, void* buf, size_t count);

/**
 * @brief  管道写
 * @param  int
 * @param  int
 * @return int
 */
int mypipe_write(mypipe_t* pipe, const void* buf, size_t size);

#endif // __MYPIPE_H__