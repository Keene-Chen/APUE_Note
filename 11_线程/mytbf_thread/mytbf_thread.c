/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.21-9:50:33
 * Description: mytbf 多线程重构版
 * !BUG: 当前程序还是会忙等,使用异步事件的查询法
 */

/**
 * * int pthread_once(pthread_once_t* once_control, void(*init_routine)(void))
 * @brief 用于控制函数只执行一次
 * @param once_control 用于控制只执行一次的变量,一般初始值为PTHREAD_ONCE_INIT
 * @param void(*init_routine)(void) 被控制的函数指针
 * @return 成功返回0,失败返回错误码
 */

#include "mytbf.h"

/* 令牌桶结构体定义 */
typedef struct {
    int cps;                     // 流控大小
    int burst;                   // 流控上限
    int token;                   // 令牌值
    pthread_mutex_t mutex_token; // 锁token
    int pos;                     // 在该结构在数组中的位置
} mytbf_st;

static mytbf_st* job[MYTBF_MAX];  // 工作数组,存放令牌桶结构的数组
static pthread_t tid;             // 线程标识
static pthread_mutex_t mutex_job; // 锁工作数组
// static int inited = 0;
static pthread_once_t init_once; // 用于控制函数只执行一次的变量

/* 获取数组中空元素的下标 */
static int get_free_pos_unlocked(void)
{
    for (int i = 0; i < MYTBF_MAX; i++) {
        if (job[i] == NULL)
            return i;
    }

    return -1;
}

/* 线程处理函数（回调函数） */
static void* thr_handler(void* arg)
{
    struct timespec ts;
    ts.tv_sec  = 1;
    ts.tv_nsec = 0;

    while (1) {
        pthread_mutex_lock(&mutex_job);
        for (int i = 0; i < MYTBF_MAX; i++) {
            if (job[i] != NULL) {
                pthread_mutex_lock(&job[i]->mutex_token);
                job[i]->token += job[i]->cps;
                if (job[i]->token > job[i]->burst)
                    job[i]->token = job[i]->burst;
                pthread_mutex_unlock(&job[i]->mutex_token);
            }
        }
        pthread_mutex_unlock(&mutex_job);
        nanosleep(&ts, NULL);
    }
    pthread_exit(NULL);
}

/* 卸载线程处理模块 */
static void module_unload(void)
{
    pthread_cancel(tid);
    pthread_join(tid, NULL);
    for (int i = 0; i < MYTBF_MAX; i++) {
        if (job[i] != NULL) {
            mytbf_destroy(job[i]);
        }
        free(job[i]);
    }

    pthread_mutex_destroy(&mutex_job);
}

/* 加载线程处理模块 */
static void module_load(void)
{
    int err = pthread_create(&tid, NULL, thr_handler, NULL);
    if (err) {
        fprintf(stderr, "%s\n", strerror(err));
    }
    atexit(module_unload); // 注册模块退出函数（钩子函数）
}

mytbf_t* mytbf_init(int cps, int burst)
{
    /**
     * ? 这里可以使用第三把锁单独控制模块加载
     *
     * pthread_mutex_lock();
     * if (!inited) {
     *     module_load();
     *     inited = 1;
     * }
     * pthread_mutex_unlock();
     */
    pthread_once(&init_once, module_load);

    mytbf_st* me;
    me = malloc(sizeof(*me));
    if (me == NULL)
        return NULL;
    me->cps   = cps;
    me->burst = burst;
    me->token = 0;
    pthread_mutex_init(&me->mutex_token, NULL); // 初始化token锁

    pthread_mutex_lock(&mutex_job);
    // ! 函数跳转也要考虑是否释放锁,通常在函数后面添加 unlocked 表示用户使用时需要自行加锁。
    int index = get_free_pos_unlocked();
    if (index < 0) {
        free(me);
        pthread_mutex_unlock(&mutex_job);
        return NULL;
    }
    me->pos    = index;
    job[index] = me;
    pthread_mutex_unlock(&mutex_job);

    return me;
}

int mytbf_destroy(mytbf_t* ptr)
{
    mytbf_st* tbf = ptr; // 由于隐藏了结构体定义,所以需要将void*转换为mytbf_t*

    pthread_mutex_lock(&mutex_job);
    job[tbf->pos] = NULL; // 将当前令牌桶在数组中的位置置为NULL,防止野指针
    pthread_mutex_unlock(&mutex_job);

    pthread_mutex_destroy(&tbf->mutex_token); // 释放token锁

    free(ptr); // 释放令牌桶指针

    return 0;
}

int mytbf_fetch_token(mytbf_t* ptr, int size)
{
    mytbf_st* tbf = ptr;

    if (size <= 0)
        return -EINVAL;

    pthread_mutex_lock(&tbf->mutex_token);
    while (tbf->token <= 0) {
        pthread_mutex_unlock(&tbf->mutex_token);
        sched_yield(); // ! 让出调度器,程序会忙等
        pthread_mutex_lock(&tbf->mutex_token);
    }
    int n = tbf->token < size ? tbf->token : size; // 用户申请的令牌数量大于最大值则返回最大值
    tbf->token -= n;
    pthread_mutex_unlock(&tbf->mutex_token);

    return n;
}

int mytbf_return_token(mytbf_t* ptr, int size)
{
    mytbf_st* tbf = ptr;

    if (size <= 0) {
        return -EINVAL;
    }

    pthread_mutex_lock(&tbf->mutex_token);
    tbf->token += size;
    if (tbf->token > tbf->burst)
        tbf->token = tbf->burst;
    pthread_mutex_unlock(&tbf->mutex_token);

    return size;
}
