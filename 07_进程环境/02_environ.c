/**
 * @file    : 02_environ.c
 * @author  : KeeneChen
 * @date    : 2022.10.10-19:00:39
 * @details : 02_environ
 */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    setenv("sss", "sdsa", 0);
    printf("%s\n", getenv("sss"));

    unsetenv("sss");
    printf("%s\n", getenv("sss"));

    return 0;
}