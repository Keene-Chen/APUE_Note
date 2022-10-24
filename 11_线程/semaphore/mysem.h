#ifndef __SEM_H__
#define __SEM_H__

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef void mysem_t;

mysem_t* mysem_init(int initval);
int mysem_add(mysem_t* ptr, int n);
int mysem_sub(mysem_t* ptr, int n);
int mysem_destory(mysem_t* ptr);

#endif // __SEM_H__