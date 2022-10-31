/**
 * @file    : 02_daemon_refactor.c
 * @author  : KeeneChen
 * @date    : 2022.10.13-19:19:53
 * @details : 02_daemon_refactor
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

static int create_daemon(void)
{
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return -1;
    }
    else if (pid > 0) {
        exit(EXIT_SUCCESS); // 父进程退出
    }

    // 子进程变为守护进程
    int fd = open("/dev/null", O_RDWR);
    if (fd < 0) {
        perror("open failed");
        return -1;
    }

    // 文件重定向
    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);
    if (fd > 2)
        close(fd);

    setsid();

    // 切换当前工作目录
    chdir("/");
    umask(0);

    return 0;
}

int main(void)
{
    // 连接系统日志
    openlog("daemon", LOG_PID, LOG_DAEMON);

    // 创建守护进程
    if (create_daemon() != 0) {
        syslog(LOG_ERR, "create daemon failed");
        exit(EXIT_SUCCESS);
    }
    else {
        syslog(LOG_INFO, "create daemon succeed");
    }

    FILE* fp = fopen("/tmp/out", "w");
    if (fp == NULL)
        syslog(LOG_ERR, "fopen failed:%s", strerror(errno));

    char buf[64];

    for (int i = 0;; i++) {
        time_t stamp  = time(NULL);
        struct tm* tm = localtime(&stamp);
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);

        fprintf(fp, "%d %s\n", i, buf);
        fflush(fp);
        sleep(1);
    }

    // 关闭资源
    fclose(fp);
    closelog();

    return 0;
}