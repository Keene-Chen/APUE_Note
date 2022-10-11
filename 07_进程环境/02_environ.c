#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    setenv("sss", "sdsa", 0);
    printf("%s\n", getenv("sss"));

    unsetenv("sss");
    printf("%s\n", getenv("sss"));

    return 0;
}