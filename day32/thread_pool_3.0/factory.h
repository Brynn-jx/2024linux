#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "head.h"
#include "taskQueue.h"

// 管理线程的数据结构和方法
typedef struct{
    pthread_t *thidAddr; // 存放线程id的起始地址
    int threadNum; // 线程数目
    pthread_cond_t cond; // 条件变量
    taskQueue_t taskQueue; // 队列
    int exitFlag;//0代表运行，1代表要退出
}factory_t;

// 初始化工厂
int factoryInit(factory_t *pFactory, int threadNum);

//启动工厂
int factoryStart(factory_t *pFactory);

// 子线程函数
void *threadFunc(void* p);

//socket,bind,listen的初始化
int tcp_init(char* ip,char* port,int *sfd);

//注册监控某个描述符是否可读
int epoll_add(int epfd,int fd);

// 发送文件
int transFile(int new_fd);
#endif