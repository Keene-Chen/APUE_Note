/**
 * @file    : proto.h
 * @author  : KeeneChen
 * @date    : 2022.10.31-12:50:54
 * @details : 协议定义
 * @todo    : 实现ftp单机版
 */

#ifndef __PROTO_H__
#define __PROTO_H__

#define KEYPATH "/etc/services"
#define KEYPROJ 'a'
#define PATH_MAX 1024 // 最大路径数
#define DATA_MAX 1024 // 最大数据数

/* 定义数据包格式 */
enum { MSG_PATH = 1, MSG_DATA, MSG_EOT };

typedef struct msg_path_s {
    long mtype; // must be MSG_PATH
    char path[PATH_MAX];
} msg_path_t;

typedef struct msg_data_s {
    long mtype; // must be MSG_DATA
    char data[DATA_MAX];
    int data_len;
} msg_data_t;

typedef struct msg_eot_s {
    long mtype; // must be MSG_EOT
} msg_eot_t;

typedef union {
    long mtype;
    msg_data_t msg_data;
    msg_eot_t msg_eot;
} msg_s2c_un;

#endif // __PROTO_H__