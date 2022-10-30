#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>

int main()
{
    struct tms tms_start, tms_end;
    clock_t time_head, time_end;

    long tck = sysconf(_SC_CLK_TCK); /*获取系统时钟(1秒里有多少个)*/

    time_head = times(&tms_start);                           /*进程运行到此时的系统时钟数(总的)*/
    printf("head_time is : %f \n", time_head / (double)tck); /*此时进程所处的时间点(单位为秒)*/

    system("sleep 2"); /*睡眠2秒*/

    time_end = times(&tms_end);                            /*进程到此时的系统时钟数*/
    printf("end_time is : %f \n", time_end / (double)tck); /*此时进程所处的时间点(单位为秒)*/

    printf("%f\n", (time_end - time_head) / (double)tck);

    printf("user time is : %f \n", (tms_end.tms_utime - tms_start.tms_utime) / (double)tck); /*打印出用户进程到此所经历时间*/
    printf("systime time is : %f \n", (tms_end.tms_stime - tms_start.tms_stime) / (double)tck);
    printf("child user time is : %f \n", (tms_end.tms_cutime - tms_start.tms_cutime) / (double)tck);
    printf("child sys time is : %f \n", (tms_end.tms_cstime - tms_start.tms_cstime) / (double)tck);

    return (0);
}
