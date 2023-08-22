/**
 * @file    : 08_lockf.c
 * @author  : KeeneChen
 * @date    : 2022.10.27-11:08:54
 * @details : 08_lockf
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define PROCMAX 20
#define LINESIZE 1024
#define FILENAME "/tmp/out"

static void func_add(void)
{
    char linebuf[LINESIZE];

    FILE* fp = fopen(FILENAME, "r+");
    if (fp == NULL) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    int fd = fileno(fp); // 从流中获取文件描述符

    lockf(fd, F_LOCK, 0);
    fgets(linebuf, LINESIZE, fp);
    // sleep(1); // 放大竞争现象
    fseek(fp, 0, SEEK_SET);
    fprintf(fp, "%d\n", atoi(linebuf) + 1); // 全缓冲
    fflush(fp);                             // 防止文件未写完就关闭
    lockf(fd, F_ULOCK, 0);
    fclose(fp); // 如果在文件锁之前关闭流会造成锁失效
}

int main(void)
{
    for (int i = 0; i < PROCMAX; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) {
            func_add();
            exit(EXIT_SUCCESS);
        }
        else if (pid > 0) {
            wait(NULL);
        }
    }

    return 0;
}