/**
 * @file    : 03_mkfifo_read.c
 * @author  : KeeneChen
 * @date    : 2022.10.30-11:05:13
 * @details : 命名管道无亲缘关系进程间通信
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(void)
{
    const char* path_name = "fifo";
    if (access(path_name, R_OK) != 0) { // 判断文件是否存在,也可以使用fstat,stat,lstat
        int ret = mkfifo(path_name, 0777);
        if (ret < 0) {
            perror("mkfifo failed");
            exit(EXIT_FAILURE);
        }
    }

    int fd = open(path_name, O_RDWR);
    if (fd < 0) {
        perror("open file failed");
        exit(EXIT_FAILURE);
    }

    char buf[BUF_SIZE];

    int len = read(fd, buf, BUF_SIZE);
    write(STDOUT_FILENO, buf, len);

    close(fd);

    return 0;
}