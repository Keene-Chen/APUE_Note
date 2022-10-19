/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.18-18:56:17
 * Description: 07_daemon_sigaction
 */

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syslog.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

static FILE* fp;

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

static void daemon_exit(int signal)
{
    // 关闭资源
    fclose(fp);
    closelog();
    exit(EXIT_SUCCESS);
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

    /**
     * 注册信号处理函数,但是有可重入风险,所以使用sigaction
     * signal(SIGINT, daemon_exit);
     * signal(SIGQUIT, daemon_exit);
     * signal(SIGTERM, daemon_exit);
     */
    struct sigaction sa;
    sa.sa_handler = daemon_exit;
    sa.sa_flags   = 0;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGINT | SIGQUIT | SIGTERM);

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    fp = fopen("/tmp/out", "w");
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

    return 0;
}