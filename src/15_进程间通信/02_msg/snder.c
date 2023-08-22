/**
 * @file    : snder.c
 * @author  : KeeneChen
 * @date    : 2022.10.31-18:51:24
 * @details : snder
 */

#include "proto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    key_t key = ftok(KEYPATH, KEYPROJ);
    if (key < 0) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }

    int msgid = msgget(key, 0);
    if (msgid < 0) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    msg_t sbuf;
    sbuf.mtype = 1;
    strcpy(sbuf.name, "zanshang");
    sbuf.math    = rand() % 100;
    sbuf.chinese = rand() % 100;

    int ret = msgsnd(msgid, &sbuf, sizeof(sbuf) - sizeof(long), 0);
    if (ret < 0) {
        perror("msgrcv failed");
        exit(EXIT_FAILURE);
    }

    puts("send over!");

    return 0;
}