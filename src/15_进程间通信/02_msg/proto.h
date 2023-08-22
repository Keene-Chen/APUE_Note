/**
 * @file    : proto.h
 * @author  : KeeneChen
 * @date    : 2022.10.30-21:13:34
 * @details : proto
 */

#ifndef __PROTO_H__
#define __PROTO_H__

#define KEYPATH "/etc/services"
#define KEYPROJ 'g'
#define BUF_SIZE 32

typedef struct msg_s {
    long mtype;
    char name[BUF_SIZE];
    int math;
    int chinese;
} msg_t;

#endif // __PROTO_H__