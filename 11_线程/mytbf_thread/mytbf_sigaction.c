/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.17-21:24:18
 * Description: 重构mytbf,由于signal函数无法区分信号的来源
 * 容易被攻击,所以使用sigaction重构
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

static struct sigaction old_sa; // 保存信号之前的行为
static struct itimerval old_itv;

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
static void alrm_action(int signal, siginfo_t* infop, void* unused)
{
    // 判断信号来源是否来自内核
    if (infop->si_code != SI_KERNEL)
        return;

    struct itimerval itv;
    itv.it_interval.tv_sec  = 1;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec     = 1;
    itv.it_value.tv_usec    = 0;
    if (setitimer(ITIMER_REAL, &itv, NULL) < 0) {
        perror("setitimer failed");
        exit(EXIT_FAILURE);
    }

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
    sigaction(SIGALRM, &old_sa, NULL);

    struct itimerval itv;
    itv.it_interval.tv_sec  = 0;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec     = 0;
    itv.it_value.tv_usec    = 0;
    if (setitimer(ITIMER_REAL, &old_itv, NULL) < 0) {
        perror("setitimer failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < MYTBF_MAX; i++) {
        free(job[i]);
    }
}

/* 加载信号处理模块 */
static void module_load(void)
{
    struct sigaction sa;
    sa.sa_sigaction = alrm_action;
    sa.sa_flags     = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGALRM, &sa, &old_sa) < 0) {
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }

    struct itimerval itv;
    itv.it_interval.tv_sec  = 1;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec     = 1;
    itv.it_value.tv_usec    = 0;
    if (setitimer(ITIMER_REAL, &itv, &old_itv) < 0) {
        perror("setitimer failed");
        exit(EXIT_FAILURE);
    }

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
