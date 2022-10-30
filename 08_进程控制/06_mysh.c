/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.12-19:19:19
 * Description: 05_mysh
 */
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct {
    glob_t globres;
} cmd_t;

static void prompt(void)
{
    printf("kcsh-0.1$ ");
}

static void parse(char* line, cmd_t* res)
{
    char* tok;
    int count = 0;
    while (1) {
        tok = strsep(&line, " \t\n");
        if (tok == NULL)
            break;
        if (tok[0] == '\0')
            continue;
        glob(tok, GLOB_NOCHECK | GLOB_APPEND * count, NULL, &res->globres);
        count = 1;
    }
}

int main(void)
{
    char* linebuf       = NULL;
    size_t linebuf_size = 0;
    cmd_t cmd;

    while (1) {
        prompt();

        if (getline(&linebuf, &linebuf_size, stdin) < 0) {
            break;
        }

        parse(linebuf, &cmd);

        if (0) {
            // TDO: 内部命令不处理
        }
        else {
            // 处理外部命令
            pid_t pid = fork();
            if (pid < 0) {
                perror("fork failed");
                exit(EXIT_FAILURE);
            }
            else if (pid == 0) {
                execvp(cmd.globres.gl_pathv[0], cmd.globres.gl_pathv);
                perror("execvp");
                exit(EXIT_FAILURE);
            }
            else if (pid > 0) {
                wait(NULL);
            }
        }
    }

    return 0;
}