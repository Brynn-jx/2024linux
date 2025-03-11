#ifndef __TASKQUEUE_H__ // 防止头文件重定义
#define __TASKQUEUE_H__
#include "head.h"

// 存放的是等待线程的任务
typedef struct task_n{
    int new_fd;
    int user_id; // 用户id
    char path[1024];    // 存放当前路径
    int path_id; // 当前路径的id，主要是为了改变文件目录做准备
    struct task_n* pnext;
}task_t;

typedef struct{
    task_t *front;  // 队列头，设计的队列是没有头节点的
    task_t *rear;
    int queueSize;
    pthread_mutex_t mutex; // 锁

}taskQueue_t;

// 入队函数
int taskEnQueue(taskQueue_t *pQueue, int new_fd);

// 出队函数
int taskDeQueue(taskQueue_t *pQueue, task_t **pnew);
#endif