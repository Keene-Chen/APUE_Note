/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.17-21:24:29
 * Description: mytbf_test
 */

#include "mytbf.h"

#define CPS 10
#define BUF_SIZE 1024
#define BURST 100 // 最大权限值

int main(int argc, char const* argv[])
{
    int src_fd;                 // 读文件描述符
    int dest_fd        = 0;     // 写文件描述符
    char buf[BUF_SIZE] = { 0 }; // 缓冲区
    int len = 0, ret = 0, pos;

    // 判断命令行参数个数
    if (argc < 2) {
        fprintf(stderr, "usage...\n");
        exit(EXIT_FAILURE);
    }

    mytbf_t* tbf = mytbf_init(CPS, BURST);
    if (tbf == NULL) {
        fprintf(stderr, "mytbf_init failed %s:\n");
        exit(EXIT_FAILURE);
    }

    // 保证文件打开成功
    do {
        src_fd = open(argv[1], O_RDONLY);
        if (src_fd < 0) {
            if (errno != EINTR) { // 防止阻塞的系统调用被中断
                fprintf(stderr, "open %s failed\n", argv[1]);
                exit(EXIT_FAILURE);
            }
        }
    } while (src_fd < 0);

    // 读写操作
    while (1) {
        // 获取令牌
        int size = mytbf_fetch_token(tbf, BUF_SIZE);
        if (size < 0) {
            fprintf(stderr, "mytbf_fetch_token failed %s:\n", strerror(-size));
            exit(EXIT_FAILURE);
        }

        while ((len = read(src_fd, buf, size)) < 0) {
            if (errno == EINTR) // 如果系统调用被中断就又继续执行
                continue;
            else {
                perror("read failed");
                break;
            }
        }

        if (len == 0) {
            break;
        }

        // 归还令牌
        if (size - len > 0)
            mytbf_return_token(tbf, size - len);

        pos = 0;
        while (len > 0) {
            ret = write(dest_fd, buf + pos, len);
            if (ret < 0) {
                if (errno == EINTR)
                    continue;
                else {
                    perror("write failed");
                    break;
                }
            }

            pos += ret;
            len -= ret;
        }
    }

    close(src_fd);
    mytbf_destroy(tbf);

    return 0;
}