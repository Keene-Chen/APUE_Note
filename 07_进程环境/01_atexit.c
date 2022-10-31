/**
 * @file    : 01_atexit.c
 * @author  : KeeneChen
 * @date    : 2022.10.10-18:59:39
 * @command : echo $? 查看程序结束返回值
 */

#include <stdio.h>
#include <stdlib.h>

void exit_handle(void)
{
    printf("终止处理程序\n");
}

void exit_handle_1(void)
{
    printf("终止处理程序_1\n");
}
void exit_handle_2(void)
{
    printf("终止处理程序_2\n");
}

int main(void)
{
    atexit(exit_handle);
    atexit(exit_handle_1);
    atexit(exit_handle_2);
    printf("程序执行完\n");
    // exit(EXIT_FAILURE);
    _Exit(0);
}