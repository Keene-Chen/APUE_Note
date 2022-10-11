/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.11-14:35:30
 * Description: mydate
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_SIZE 128

int main(int argc, char* const argv[])
{
    char fmtstr[MAX_SIZE] = { 0 };
    char buf[MAX_SIZE] = { 0 };
    char ch;
    FILE* fp = stdout;

    // 获取时间
    time_t stamp = time(NULL);
    struct tm* tm = localtime(&stamp);

    while (1) {
        // 解析命令行参数 -表示非选项参数,成功返回1 :表示描述参数的参数
        ch = getopt(argc, argv, "-H:MSy:md");
        if (ch < 0)
            break;

        // 处理参数
        switch (ch) {
        case 1: {
            if (fp == stdout) {
                fp = fopen(argv[optind - 1], "w+");
                if (fp == NULL) {
                    fprintf(stderr, "open failed\n");
                    fp = stdout;
                }
            }
            break;
        }
        case 'H': {
            if (strcmp(optarg, "12") == 0)
                strcat(fmtstr, "%I(%p) ");
            else if (strcmp(optarg, "24") == 0)
                strcat(fmtstr, "%H ");
            else
                fprintf(stderr, "invalid argment");
            break;
        }
        case 'M':
            strcat(fmtstr, "%M ");
            break;
        case 'S':
            strcat(fmtstr, "%S ");
            break;
        case 'y':
            strcat(fmtstr, "%y ");
            break;
        case 'm':
            strcat(fmtstr, "%m ");
            break;
        case 'd':
            strcat(fmtstr, "%d ");
            break;

        default:
            strcat(fmtstr, "%y-%m-%d %H:%M:%S");
            break;
        }
    }

    strcat(fmtstr, "\n");
    strftime(buf, sizeof(buf), fmtstr, tm);
    fputs(buf, fp);

    if (fp != stdout)
        fclose(fp);

    return 0;
}