/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.27-11:27:17
 * Description: 09_readv_writev
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>

int main(void)
{
    char* str1 = "hello\n";
    char* str2 = "world\n";

    struct iovec iovec[2];
    iovec[0].iov_base = str1;
    iovec[0].iov_len  = strlen(str1);
    iovec[1].iov_base = str1;
    iovec[1].iov_len  = strlen(str2);

    int fd = open("1", O_RDWR | O_CREAT);
    writev(fd, iovec, 2);

    return 0;
}