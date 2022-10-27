/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.27-10:24:47
 * Description: 07_shared_memory 有血缘关系的进程间通信,父子进程使用mmap通信,父进程读子进程写
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MEM_SIZE 1024

int main(void)
{
    char* ptr = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        munmap(ptr, MEM_SIZE);
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) { // child write
        strcpy(ptr, "hello");
        munmap(ptr, MEM_SIZE);
        exit(EXIT_SUCCESS);
    }
    else if (pid > 0) { // parent read
        wait(NULL);
        puts(ptr);
        munmap(ptr, MEM_SIZE);
        exit(EXIT_SUCCESS);
    }

    return 0;
}