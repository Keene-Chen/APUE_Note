/**
 * @file    : sem.c
 * @author  : KeeneChen
 * @date    : 2022.10.31-15:50:58
 * @details : sem
 */

/**
 * 信号量数组相关API
 * @include #include <sys/ipc.h>
 *          #include <sys/sem.h>
 *          #include <sys/types.h>
 * 
 * * int semget(key_t key, int nsems, int semflg);
 * @brief 得到一个信号量集标识符或创建一个信号量集对象
 * @param key    0(IPC_PRIVATE),大于0的32位整数:视参数semflg来确定操作,通常要求此值来源于ftok返回的IPC键值
 * @param nsems  创建信号量集中信号量的个数,该参数只在创建信号量集时有效
 * @param semflg 0: 取信号量集标识符,若不存在则函数会报错
 *               IPC_CREAT: 当semflg&IPC_CREAT为真时,如果内核中不存在键值与key相等的信号量集,则新建一个信号量集;
 *                          如果存在这样的信号量集,返回此信号量集的标识符
 *               IPC_CREAT|IPC_EXCL: 如果内核中不存在键值与key相等的信号量集,则新建一个消息队列;
 *                                   如果存在这样的信号量集则报错
 * @return int 成功:返回信号量集的标识符,出错:-1,错误原因存于error中
 *
 * * int semctl(int semid, int semnum, int cmd, ...);
 * @brief 得到一个信号量集标识符或创建一个信号量集对象并返回信号量集标识符
 * @param semid  信号量集标识符
 * @param semnum 信号量集数组上的下标，表示某一个信号量
 * @param cmd    IPC_STAT 从信号量集上检索semid_ds结构，并存到semun联合体参数的成员buf的地址中
 *               IPC_SET  设置一个信号量集合的semid_ds结构中ipc_perm域的值，并从semun的buf中取出值
 *               IPC_RMID 从内核中删除信号量集合
 *               GETALL   从信号量集合中获得所有信号量的值，并把其整数值存到semun联合体成员的一个指针数组中
 *               GETNCNT  返回当前等待资源的进程个数
 *               GETPID   返回最后一个执行系统调用semop()进程的PID
 *               GETVAL   返回信号量集合内单个信号量的值
 *               GETZCNT  返回当前等待100%资源利用的进程个数
 *               SETALL   与GETALL正好相反
 *               SETVAL   用联合体中val成员的值设置信号量集合中单个信号量的值
 * @return int 成功: 大于或等于0,出错:-1,并设置errno
 *
 * * int semop(int semid, struct sembuf *sops, size_t nsops);
 * @brief 对信号量集标识符为semid中的一个或多个信号量进行P操作或V操作
 * @param semid  指向进行操作的信号量集结构体数组的首地址
 * @param sops   指向进行操作的信号量集结构体数组的首地址
 * @param nsops  进行操作信号量的个数，即sops结构变量的个数，需大于或等于1。最常见设置此值等于1，只完成对一个信号量的操作
 * @return int 成功: 返回信号量集的标识符,出错:-1,并设置errno
 *
 * * struct sembuf {
 * * short semnum; 信号量集合中的信号量编号,0代表第1个信号量
 * * short val;    若val>0进行V操作信号量值加val,表示进程释放控制的资源
 *                若val<0进行P操作信号量值减val,若(semval-val)<0（semval为该信号量值）,则调用进程阻塞,直到资源可用;
 *                若设置IPC_NOWAIT不会睡眠,进程直接返回EAGAIN错误
 *                若val==0时阻塞等待信号量为0,调用进程进入睡眠状态,直到信号值为0;
 *                若设置IPC_NOWAIT,进程不会睡眠,直接返回EAGAIN错误
 * * short flag;   0 设置信号量的默认操作
 *                IPC_NOWAIT设置信号量操作不等待
 *                SEM_UNDO 选项会让内核记录一个与调用进程相关的UNDO记录,如果该进程崩溃,
 *                         则根据这个进程的UNDO记录自动恢复相应信号量的计数值
 * * };
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PROCMAX 20
#define LINESIZE 1024
#define FILENAME "/tmp/out"

static int semid;

static void P(void)
{
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op  = -1;
    op.sem_flg = 0;

    while (semop(semid, &op, 1) < 0) {
        if (errno != EINTR || errno != EAGAIN) {
            perror("semop failed");
            exit(EXIT_FAILURE);
        }
    }
}

static void V(void)
{
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op  = 1;
    op.sem_flg = 0;

    if (semop(semid, &op, 1) < 0) {
        perror("semop failed");
        exit(EXIT_FAILURE);
    }
}

static void func_add(void)
{
    char linebuf[LINESIZE];

    FILE* fp = fopen(FILENAME, "r+");
    if (fp == NULL) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    P(); // 获取资源量
    fgets(linebuf, LINESIZE, fp);
    sleep(1);                               // 放大竞争现象
    fseek(fp, 0, SEEK_SET);                 // 相对偏移量查找字节数 SEEK_SET 文件头
    fprintf(fp, "%d\n", atoi(linebuf) + 1); // 全缓冲
    fflush(fp);                             // 防止文件未写完就关闭
    V();                                    // 归还资源量
    fclose(fp);                             // 如果在文件锁之前关闭流会造成锁失效
}

int main(void)
{
    // 获取semid,有亲缘关系所以使用匿名方式,否则使用ftok()
    semid = semget(IPC_PRIVATE, 1, 0600);
    if (semid < 0) {
        perror("semget failed");
        exit(EXIT_FAILURE);
    }

    // 初始化信号量数组,类似互斥锁,设置下标0的信号量的资源总量为1
    int ret = semctl(semid, 0, SETVAL, 1);
    if (ret < 0) {
        perror("semctl failed");
        exit(EXIT_FAILURE);
    }

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

    // 销毁当前信号量数组
    semctl(semid, 0, IPC_RMID);

    return 0;
}