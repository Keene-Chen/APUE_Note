#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    alarm(5);
    alarm(10);
    alarm(1);

    while (1)
        ;

    return 0;
}