/**
 * @file    : 04_5sec_time.c
 * @author  : KeeneChen
 * @date    : 2022.10.17-10:29:31
 * @details : 计时5s time 实现版本
 * @command : gcc 04_5sec_time.c && time -p ./a.out
 * count: 1341882153
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    int64_t count = 0;
    time_t end    = time(NULL) + 5;

    while (time(NULL) < end)
        count++;
    printf("%ld\n", count);

    return 0;
}