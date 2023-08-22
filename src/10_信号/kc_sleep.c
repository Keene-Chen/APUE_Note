#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>

void kc_sleep(unsigned int msec)
{
    struct timespec timeout;
    int rc;

    timeout.tv_sec = msec / 1000;
    timeout.tv_nsec = (msec % 1000) * 1000 * 1000;

    do
        rc = nanosleep(&timeout, &timeout);
    while (rc == -1 && errno == EINTR);

    assert(rc == 0);
}

int main(void)
{
    for (int i = 0;; i++) {
        kc_sleep(1000);
        printf("%d\n", i);
    }

    return 0;
}