/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.17-14:57:12
 * Description: 03_mycat
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char const* argv[])
{
    char buf[BUF_SIZE] = { 0 };

    if (argc < 2) {
        fprintf(stderr, "usage...\n");
        exit(EXIT_FAILURE);
    }

    int src = open(argv[1], O_RDONLY);
    if (src < 0) {
        fprintf(stderr, "open %s failed\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    int len = 0, ret = 0;
    while (1) {
        len = read(src, buf, BUF_SIZE);
        if (len < 0) {
            perror("read failed");
            exit(EXIT_FAILURE);
        }
        else if (len == 0) {
            break;
        }
        ret = write(STDOUT_FILENO, buf, len);
        if (ret < 0) {
            perror("write failed");
            break;
        }
    }

    close(src);

    return 0;
}