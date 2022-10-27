/**
 * Author     : KeeneChen
 * DateTime   : 2022.10.27-09:30:05
 * Description: 06_mmap
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char const* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "usuage....\n");
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    struct stat statres;
    fstat(fd, &statres);
    int64_t count = 0;

    char* str = mmap(NULL, statres.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (str == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }
    close(fd);
    for (int i = 0; i < statres.st_size; i++) {
        if (str[i] == 'a')
            count++;
    }
    printf("%d\n", count);

    munmap(str, statres.st_size);

    return 0;
}