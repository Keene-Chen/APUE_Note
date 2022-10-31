/**
 * @file    : 05_demo_competition_issues.c
 * @author  : KeeneChen
 * @date    : 2022.10.20-13:44:12
 * @details : 05_demo_competition_issues
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define THRMAX 20
#define LINESIZE 1024
#define FILENAME "/tmp/out"

static void* thr_add(void* arg)
{
    char linebuf[LINESIZE];

    FILE* fp = fopen(FILENAME, "r+");
    if (fp == NULL) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    fgets(linebuf, LINESIZE, fp);
    // sleep(1); // 放大竞争现象
    fseek(fp, 0, SEEK_SET);
    fprintf(fp, "%d\n", atoi(linebuf) + 1);
    fclose(fp);

    pthread_exit(NULL);
}

int main(void)
{
    pthread_t tid[THRMAX];
    int err;

    for (int i = 0; i < THRMAX; i++) {
        err = pthread_create(tid + i, NULL, thr_add, NULL);
        if (err != 0) {
            fprintf(stderr, "pthread_create failed: %s", strerror(err));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < THRMAX; i++) {
        pthread_join(tid[i], NULL);
    }

    return 0;
}