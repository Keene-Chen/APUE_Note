#include <setjmp.h>
#include <stdio.h>

static jmp_buf env;

static void a(void);
static void b(void);
static void c(void);
static void d(void);

int main(void)
{
    printf("%s: begin\n", __FUNCTION__);
    printf("%s: call a()\n", __FUNCTION__);
    a();
    printf("%s: a() return\n", __FUNCTION__);
    printf("%s: end\n", __FUNCTION__);

    return 0;
}

static void a(void)
{
    printf("%s: begin\n", __FUNCTION__);
    int ret = setjmp(env);
    if (ret == 0) {
        printf("%s: call b()\n", __FUNCTION__);
        b();
        printf("%s: b() return\n", __FUNCTION__);
    } else {
        printf("%s Jump Succeed: %d\n", __FUNCTION__, ret);
    }
    printf("%s: end\n", __FUNCTION__);
}

static void b(void)
{
    printf("%s: begin\n", __FUNCTION__);
    printf("%s: call c()\n", __FUNCTION__);
    c();
    printf("%s: c() return\n", __FUNCTION__);
    printf("%s: end\n", __FUNCTION__);
}

static void c(void)
{
    printf("%s: begin\n", __FUNCTION__);
    printf("%s: call d()\n", __FUNCTION__);
    d();
    printf("%s: d() return\n", __FUNCTION__);
    printf("%s: end\n", __FUNCTION__);
}

static void d(void)
{
    printf("%s: begin\n", __FUNCTION__);
    printf("Jump Now!\n");
    longjmp(env, 6);
    printf("%s: end\n", __FUNCTION__);
}