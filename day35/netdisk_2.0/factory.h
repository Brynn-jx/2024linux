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
    REGISTER_FAILED
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

// 发送任何数据
int recv_protocol(int new_fd,int *control_code, void *pdat);

// 接受任何数据
int send_protocol(int new_fd,int control_code, void *pdata, int send_size);

// 初始化mysql链接
int mysql_connet(MYSQL **conn);

// 查询用户名是否存在
int check_username_exist(MYSQL *conn, char *username);

// 插入用户
uint64_t insert_new_user(MYSQL *conn, user_passwd_t *user_passwd);

// 随机生成盐值
void generate_random_string(char *random_string);

// 检查用户名，密码是否存在
int check_login(MYSQL *conn, user_passwd_t *user_passwd);
#endif