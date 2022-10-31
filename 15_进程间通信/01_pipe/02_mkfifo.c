/**
 * @file    : 02_mkfifo.c
 * @author  : KeeneChen
 * @date    : 2022.10.31-18:47:10
 * @details : 命名管道有无亲缘关系都可使用
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

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        int len = read(fd, buf, BUF_SIZE);
        write(STDOUT_FILENO, buf, len);
    }
    else if (pid > 0) {
        write(fd, "hello\n", 6);
        wait(NULL);
    }

    close(fd);

    return 0;
}