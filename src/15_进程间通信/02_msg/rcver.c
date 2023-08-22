/**
 * @file    : rcver.c
 * @author  : KeeneChen
 * @date    : 2022.10.30-21:14:34
 * @details : rcver
 * @command : ipcs  查看进程间通信相关信息
 *            ipcrm 移除相关进程间通信内容
 */

/**
 * 消息队列相关API
 * @include #include <sys/ipc.h>
 *          #include <sys/msg.h>
 *          #include <sys/types.h>
 *
 * * int msgget(key_t, key, int msgflg);
 * @brief 创建和访问一个消息队列
 * @param key 多个进程可通key值来访问同一个消息队列,通过ftok()获取
 * @param msgflag 权限标志位,IPC_CREAT|0600
 * @return int 返回一个以key命名的消息队列的标识符(非零整数),失败时返回-1
 *
 * * int msgrcv(int msgid, void *msg_ptr, size_t msg_sz, long int msgtype, int msgflg);
 * @brief 从一个消息队列获取消息
 * @param msgid   消息队列标识符
 * @param msg_ptr 指向准备发送消息的指针
 * @param msg_sz  是msg_ptr指向的消息的长度,注意是消息的长度,而不是整个结构体的长度,
 *                也就是说msg_sz是不包括长整型消息类型成员变量的长度
 * @param msgtype 可以实现一种简单的接收优先级,如果msgtype为0,就获取队列中的第一个消息
 *                如果它的值大于零,将获取具有相同消息类型的第一个信息
 *                如果它小于零,就获取类型等于或小于msgtype的绝对值的第一个消息
 * @param msgflg  用于控制当前消息队列满或队列消息到达系统范围的限制时将要发生的事情
 * @return int 成功时,该函数返回放到接收缓存区中的字节数,消息被复制到由msg_ptr指向的用户分配的缓存区中,
 *             然后删除消息队列中的对应消息。失败时返回-1
 *
 * * int msgsend(int msgid, const void *msg_ptr, size_t msg_sz, int msgflg);
 * @brief 把消息添加到消息队列中
 * @param msgid   消息队列标识符
 * @param msg_ptr 指向准备发送消息的指针
 * @param msg_sz  是msg_ptr指向的消息的长度,注意是消息的长度,而不是整个结构体的长度,
 *                也就是说msg_sz是不包括长整型消息类型成员变量的长度
 * @param msgflg  用于控制当前消息队列满或队列消息到达系统范围的限制时将要发生的事情
 * @return int 成功: 消息数据的一分副本将被放到消息队列中,并返回0,失败时返回-1
 *
 * * int msgctl(int msgid, int command, struct msgid_ds *buf);
 * @brief 把消息添加到消息队列中
 * @param msgid   消息队列标识符
 * @param command IPC_STAT:把msgid_ds结构中的数据设置为消息队列的当前关联值,即用消息队列的当前关联值覆盖msgid_ds的值
 *                IPC_SET:如果进程有足够的权限,就把消息列队的当前关联值设置为msgid_ds结构中给出的值
 *                IPC_RMID:删除消息队列
 * @param buf     buf是指向msgid_ds结构的指针，它指向消息队列模式和访问权限的结构
 * @param msgflg  用于控制当前消息队列满或队列消息到达系统范围的限制时将要发生的事情
 * @return int 成功时返回0，失败时返回-1
 */

#include "proto.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

static int msgid;

void handler(int sig)
{
    msgctl(msgid, IPC_RMID, NULL);
}

int main(void)
{
    signal(SIGINT, handler);
    key_t key = ftok(KEYPATH, KEYPROJ);
    if (key < 0) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }

    msgid = msgget(key, IPC_CREAT | 0600);
    if (msgid < 0) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    msg_t rbuf;
    int ret;
    while (1) {
        ret = msgrcv(msgid, &rbuf, sizeof(rbuf) - sizeof(long), 0, 0);
        if (ret < 0) {
            perror("msgrcv failed");
            exit(EXIT_FAILURE);
        }

        printf("name: %s\n", rbuf.name);
        printf("math: %d\n", rbuf.math);
        printf("chinese: %d\n", rbuf.chinese);
    }

    return 0;
}