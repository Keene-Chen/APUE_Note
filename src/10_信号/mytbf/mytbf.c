/**
 * @file    : mytbf.c
 * @author  : KeeneChen
 * @date    : 2022.10.17-21:24:18
 * @details : mytbf
 */

#include "mytbf.h"

/* 令牌桶结构体定义 */
typedef struct {
    int cps;   // 流控大小
    int burst; // 流控上限
    int token; // 令牌值
    int pos;   // 在该结构在数组中的位置
} mytbf_st;

static mytbf_st* job[MYTBF_MAX]; // 工作数组,存放令牌桶结构的数组
static int inited = 0;

typedef void (*sighandler_t)(int);     // signal函数返回值,函数指针
static sighandler_t alrm_handler_save; // 保存alarm之前的行为,默认为终止程序

/* 获取数组中空元素的下标 */
static int get_free_pos(void)
{
    for (int i = 0; i < MYTBF_MAX; i++) {
        if (job[i] == NULL)
            return i;
    }

    return -1;
}

/* alarm处理函数（回调函数） */
static void alrm_handler(int s)
{
    alarm(1); // 触发下一次alarm信号

    for (int i = 0; i < MYTBF_MAX; i++) {
        if (job[i] != NULL) {
            job[i]->token += job[i]->cps;
            if (job[i]->token > job[i]->burst)
                job[i]->token = job[i]->burst;
        }
    }
}

/* 卸载信号处理模块 */
static void module_unload(void)
{
    signal(SIGALRM, alrm_handler_save);
    alarm(0);
    for (int i = 0; i < MYTBF_MAX; i++) {
        free(job[i]);
    }
}

/* 加载信号处理模块 */
static void module_load(void)
{
    alrm_handler_save = signal(SIGALRM, alrm_handler);
    alarm(1);
    atexit(module_unload); // 注册信号退出函数（钩子函数）
}

mytbf_t* mytbf_init(int cps, int burst)
{
    mytbf_st* me;

    if (!inited) {
        module_load();
        inited = 1;
    }

    int index = get_free_pos();
    if (index < 0)
        return NULL;

    me = malloc(sizeof(*me));
    if (me == NULL)
        return NULL;
    me->cps   = cps;
    me->burst = burst;
    me->token = 0;
    me->pos   = index;

    job[index] = me;

    return me;
}

int mytbf_destroy(mytbf_t* ptr)
{
    mytbf_st* tbf = ptr;  // 由于隐藏了结构体定义,所以需要将void*转换为mytbf_t*
    job[tbf->pos] = NULL; // 将当前令牌桶在数组中的位置置为NULL,防止野指针
    free(ptr);            // 释放令牌桶指针
    return 0;
}

int mytbf_fetch_token(mytbf_t* ptr, int size)
{
    mytbf_st* tbf = ptr;

    if (size <= 0)
        return -EINVAL;

    while (tbf->token <= 0)
        pause(); // 等待alarm信号到来,避免程序无意义的消耗CPU资源

    int n = tbf->token < size ? tbf->token : size; // 用户申请的令牌数量大于最大值则返回最大值

    tbf->token -= n;

    return n;
}

int mytbf_return_token(mytbf_t* ptr, int size)
{
    mytbf_st* tbf = ptr;

    if (size <= 0) {
        return -EINVAL;
    }

    tbf->token += size;
    if (tbf->token > tbf->burst)
        tbf->token = tbf->burst;

    return size;
}
