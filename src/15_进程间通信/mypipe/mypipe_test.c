/**
 * @file   : mypipe_test.c
 * @author : KeeneChen
 * @date   : 2022.10.28-10:31:15
 * @bug    : 测试用例为能执行成功
 */

#include "mypipe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    char buf[1024]    = "sss";
    mypipe_t* pipe_rd = mypipe_init();
    // mypipe_t* pipe_wr = mypipe_init();
    puts("ss");
    // mypipe_register(pipe_rd, PIPE_READ);
    // mypipe_register(pipe_wr, PIPE_WRITE);
    mypipe_write(pipe_rd, "hello", 5);
    puts("ss");
    mypipe_read(pipe_rd, buf, 5);
    puts(buf);
    // mypipe_unregister(pipe_rd, PIPE_READ);
    // mypipe_unregister(pipe_wr, PIPE_WRITE);
    mypipe_destroy(pipe_rd);
    // mypipe_destroy(pipe_wr);

    return 0;
}