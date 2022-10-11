#include <dlfcn.h>
#include <gnu/lib-names.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    void* handle = dlopen(LIBM_SO, RTLD_LAZY);
    if (handle == NULL) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    double (*sin)(double);
    sin = (double (*)(double))dlsym(handle, "sin");

    char* error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }

    printf("%f\n", sin(90 * 3.14 / 180));
    dlclose(handle);

    return 0;
}
