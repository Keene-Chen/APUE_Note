/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.25-16:37:21
 * Description: select当定时器使用
 */

#include <stdio.h>
#include <sys/select.h>
#include <time.h>

int main(void)
{
    printf("start\n");
    struct timeval tiv;
    tiv.tv_sec  = 1;
    tiv.tv_usec = 0;
    select(-1, NULL, NULL, NULL, &tiv);
    printf("end\n");

    return 0;
}