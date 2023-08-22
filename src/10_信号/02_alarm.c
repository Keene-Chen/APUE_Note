/**
 * @file    : 02_alarm.c
 * @author  : KeeneChen
 * @date    : 2022.10.14-13:50:31
 * @details : 02_alarm
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    alarm(5);
    alarm(10);
    alarm(1);

    while (1)
        ;

    return 0;
}