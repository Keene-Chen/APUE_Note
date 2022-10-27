/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.25-16:11:29
 * Description: relayer_test
 */

#include "relayer.h"

#define TTY1 "1"
#define TTY2 "2"

static int open_file(const char* path, int flag)
{
    int fd = open(path, O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "%s open failed", path);
        exit(EXIT_FAILURE);
    }
    return fd;
};

int main(void)
{
    int fd[3] = { 0 };

    //假设用户使用阻塞的方式打开设备
    fd[1] = open_file(TTY1, O_RDWR);
    write(fd[1], "tty1\n", 5);

    fd[2] = open_file(TTY2, O_RDWR | O_NONBLOCK);
    write(fd[2], "tty2\n", 5);

    int job1 = rel_job_add(fd[1], fd[2]);
    if (job1 < 0) {
        fprintf(stderr, "rel_job_add failed:%s\n", strerror(-job1));
        exit(EXIT_FAILURE);
    }
    while (1)
        pause();

    close(fd[1]);
    close(fd[2]);

    return 0;
}