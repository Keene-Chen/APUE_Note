/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.13-10:26:38
 * Description: 06_mysu
 * Command: mysu 0 cat /etc/shadow
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char* const argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage  : mysu uid command ...\n");
        fprintf(stderr, "example: mysu 0 cat /etc/shadow\n");
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        // 设置有效用户ID
        setuid(atoi(argv[1]));
        execvp(argv[2], argv + 2);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) {
        wait(NULL);
    }

    return 0;
}