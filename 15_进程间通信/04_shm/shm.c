/**
 * @file    : shm.c
 * @author  : KeeneChen
 * @date    : 2022.10.31-19:14:29
 * @details : shm
 */

/**
 * 共享内存相关API
 *
 * @include #include <sys/ipc.h>
 *          #include <sys/shm.h>
 *          #include <sys/types.h>
 *
 * * int shmget (key_t key, int size, int shmflg);
 * @brief 创建共享内存
 * @param key    标识共享内存的键值,可以取非负整数或IPC_PRIVATE
 * @param size   共享内存的大小,以字节为单位
 * @param shmflg 权限标志,与文件的读写权限一样
 * @return int 成功,则返回共享内存标识符,否则返回-1并设置errno
 *
 * * int shmat (int shmid, char *shmaddr, int flag);
 * @brief 映射共享内存
 * @param shmid   shmget函数返回的共享内存标识符
 * @param shmaddr 0
 * @param flag    0
 * @return int 成功,则返回共享内存映射到进程中的地址,否则返回(void*)-1并设置errno
 *
 * * int shmdt (int shmid, char *shmaddr, int flag);
 * @brief 删除映射共享内存
 * @param shmid   shmget函数返回的共享内存标识符
 * @param shmaddr 从shmat中获得
 * @param flag    0
 * @return int 成功,则返回共享内存标识符,否则返回-1,并设置errno
 *
 * * int shmctl(int shmid, int cmd, struct shmid_ds *buf);
 * @brief 操作共享内存
 * @param shmid   shmget函数返回的共享内存标识符
 * @param cmd
 * @param buf
 * @return int 成功,则返回共享内存标识符,否则返回-1,并设置errno
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MEM_SIZE 1024

int main(void)
{
    int shmid = shmget(IPC_PRIVATE, MEM_SIZE, 0600);
    if (shmid < 0) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    char* ptr;
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) { // chiled write
        ptr = shmat(shmid, NULL, 0);
        if (ptr == (void*)-1) {
            perror("shmat failed");
            exit(EXIT_FAILURE);
        }
        strcpy(ptr, "hello\n");
        shmdt(ptr);
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0) { // parent read
        wait(NULL);
        ptr = shmat(shmid, NULL, 0);
        if (ptr == (void*)-1) {
            perror("shmat failed");
            exit(EXIT_FAILURE);
        }
        puts(ptr);
        shmdt(ptr);
        shmctl(shmid, IPC_RMID, NULL);
        exit(EXIT_SUCCESS);
    }

    return 0;
}