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

// 协议火车，用于接收除了发送文件之外的所有操作
typedef struct{
    int size;   // 发送字符串的长度
    int control_code;   // 控制码，客户端和服务端集成在一起使用
    char buf[1000];
}protocol_t;

// 存放用户名和密码
typedef struct{
    char username[50];
    char password[50];
}user_passwd_t;

enum CONTROL{
    REGISTER,
    LOGIN,

    REGISTER_SUCCESS=100,//这里开始是客户端的控制码
};

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

int recv_protocol(int new_fd,int *control_code,char *pdata);

int send_protocol(int new_fd,int control_code,char *pdata);

// 根据空格分割字符串
int split_user_passwd(char* usrname_passwd,char* username,char* passwd);
#endif