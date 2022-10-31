/**
 * @file    : relayer.c
 * @author  : KeeneChen
 * @date    : 2022.10.25-16:11:10
 * @details : relayer
 * @warning : 程序是忙等状态
 */

#include "relayer.h"

#define BUF_SIZE 1024

/* 状态机状态枚举定义 */
enum {
    STATE_R = 1,
    STATE_W,
    STATE_E,
    STATE_T,
};

/* 状态机结构体定义 */
typedef struct rel_fsm_s {
    int state;          // 状态机状态
    int sfd;            // 源文件
    int dfd;            // 目标文件
    char buf[BUF_SIZE]; // 读写缓存区大小
    int len;            // 读取成功字节数
    int pos;            // 缓冲区当前位置
    char* errstr;       // 错误来源
    int64_t count;
} rel_fsm_t;

/* 任务结构体定义 */
typedef struct rel_job_s {
    int job_state;
    int fd[3];
    int fd_save[3];
    rel_fsm_t fsm_rl_wr, fsm_rr_wl;
} rel_job_t;

/* 全局任务数组 */
static rel_job_t* rel_job[REL_JOB_MAX];
/* 保护任务数组的互斥量 */
static pthread_mutex_t job_mutex = PTHREAD_MUTEX_INITIALIZER;
/* 单次初始化变量 */
static pthread_once_t init_once = PTHREAD_ONCE_INIT;

/* 状态机驱动 */
static void fsm_driver(rel_fsm_t* fsm)
{
    int ret;

    switch (fsm->state) {
    case STATE_R:
        fsm->len = read(fsm->sfd, fsm->buf, BUF_SIZE);
        if (fsm->len == 0) { // 读取字节数为0进入终止态
            fsm->state = STATE_T;
        }
        else if (fsm->len < 0) {   // 读取字节数小于0
            if (errno == EAGAIN) { // 假错重读
                fsm->state = STATE_R;
            }
            else { // 真错写入错误消息，并进入异常态
                fsm->errstr = "read failed";
                fsm->state  = STATE_E;
            }
        }
        else {
            fsm->pos   = 0;
            fsm->state = STATE_W;
        }
        break;

    case STATE_W:
        ret = write(fsm->dfd, (fsm->buf + fsm->pos), fsm->len);
        if (ret < 0) {
            if (errno == EAGAIN) {
                fsm->state = STATE_W;
            }
            else {
                fsm->errstr = "write failed";
                fsm->state  = STATE_E;
            }
        }
        else {
            fsm->pos += ret;
            fsm->len -= ret;
            if (fsm->len == 0)
                fsm->state = STATE_R;
            else
                fsm->state = STATE_W;
        }
        break;

    case STATE_E:
        perror(fsm->errstr);
        fsm->state = STATE_T;
        break;

    case STATE_T:
        // do sth
        break;

    default:
        abort();
        break;
    }
}

static void* thr_relayer(void* arg)
{
    while (1) {
        pthread_mutex_lock(&job_mutex);
        for (int i = 0; i < REL_JOB_MAX; i++) {
            if (rel_job[i] != NULL) {
                if (rel_job[i]->job_state == STATE_RUNNING) {
                    // 尝试发动状态机
                    fsm_driver(&rel_job[i]->fsm_rl_wr);
                    fsm_driver(&rel_job[i]->fsm_rr_wl);
                    if (rel_job[i]->fsm_rl_wr.state == STATE_T && rel_job[i]->fsm_rr_wl.state == STATE_T) {
                        rel_job[i]->job_state = STATE_OVER; // 两个状态机都已经停止 任务结束
                    }
                }
            }
        }
        pthread_mutex_unlock(&job_mutex);
    }
    pthread_exit(NULL);
}

static void module_load()
{
    pthread_t tid;
    int err = pthread_create(&tid, NULL, thr_relayer, NULL);
    if (err) {
        fprintf(stderr, "pthread_create failed:%s\n", strerror(err));
        exit(EXIT_FAILURE);
    }
}

static int get_free_pos_unlocked()
{
    for (int i = 0; i < REL_JOB_MAX; i++) {
        if (rel_job[i] == NULL)
            return i;
    }
    return -1;
}

int rel_job_add(int fd1, int fd2)
{
    pthread_once(&init_once, module_load);
    rel_job_t* job = mi_malloc(sizeof(rel_job_t*));
    if (job == NULL)
        return -ENOMEM;

    job->fd[1]     = fd1;
    job->fd[2]     = fd2;
    job->job_state = STATE_RUNNING;

    // 保存文件描述符之前状态
    job->fd_save[1] = fcntl(fd1, F_GETFL);
    fcntl(fd1, job->fd_save[1] | O_NONBLOCK);
    job->fd_save[2] = fcntl(fd2, F_GETFL);
    fcntl(fd2, job->fd_save[2] | O_NONBLOCK);

    job->fsm_rl_wr.state = STATE_R;
    job->fsm_rl_wr.sfd   = fd1;
    job->fsm_rl_wr.dfd   = fd2;
    job->fsm_rr_wl.state = STATE_R;
    job->fsm_rr_wl.sfd   = fd2;
    job->fsm_rr_wl.dfd   = fd1;

    pthread_mutex_lock(&job_mutex);
    int pos = get_free_pos_unlocked();
    if (pos < 0) {
        pthread_mutex_unlock(&job_mutex);
        // 恢复原来的文件描述符状态
        fcntl(job->fd[1], F_SETFL, job->fd_save[1]);
        fcntl(job->fd[2], F_SETFL, job->fd_save[2]);
        mi_free(job);
        return -ENOSPC;
    }
    rel_job[pos] = job;
    pthread_mutex_unlock(&job_mutex);

    return pos;
}

int rel_job_cancel(int id);
int rel_job_wait(int id);
int rel_job_stat(int id, rel_stat_t*);