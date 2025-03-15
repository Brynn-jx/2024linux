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
    LS,
    CD,
    PWD,
    MKDIR,
    RM,
    GETS,
    PUTS,

    REGISTER_SUCCESS=100,//这里开始是客户端的控制码
    REGISTER_FAILED,
    LOGIN_SUCCESS,
    LOGIN_FAILED,
    SUCCESS,
    FAILED
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

// 解除注册
int epoll_del(int epfd,int fd);

// 发送文件
int transFile(int new_fd, char* file_name, off_t seek_pos, off_t remain_size);

// 发送任何数据
int recv_protocol(int new_fd,int *control_code, void *pdat);

// 接受任何数据
int send_protocol(int new_fd,int control_code, void *pdata, int send_size);

void client_handle(task_t *pnew, MYSQL *conn);//处理客户端ls,cd等

void do_ls(task_t* pnew, MYSQL *conn);

void do_cd(task_t *pnew,char *path, MYSQL *conn);

void do_pwd(task_t* pnew);

void do_mkdir(task_t* pnew,MYSQL* conn, char *filename);

void do_rm(task_t *pnew, MYSQL *conn, char* rm_file_name);

void do_gets(task_t *pnew, MYSQL *conn, char *file_name_recv_size);

void do_puts(task_t* pnew, MYSQL *conn, char* file_name);

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

// 返回用户名的id
int get_user_id(MYSQL *conn, char* username);

// 查询是否在path_id 下有同名的目录
int check_dir_isexist(MYSQL* conn,int user_id,int path_id,char* dir_name);

// 创建目录
int creat_dir(MYSQL* conn,int user_id,int path_id,char* dir_name);

// 修改当前路径为上一级路径
void modify_path_slash(char *path);

// 得到上一级路径的id
int get_pre_id(MYSQL *conn, int path_id);

// 查询当前目录的名字为dir_name的id号
int check_cd_dir_isexist(MYSQL* conn,int user_id,int path_id,char* dir_name);

// 查询一行发送一行
void send_file_info(task_t* pnew, MYSQL *conn);

// 删除空文件夹和文件
int rm_file_emptydir(task_t* pnew, MYSQL *conn, char* rm_file_name);

// 接收火车
int recvn(int new_fd, void* pStart, int len);

// 查询数据库中是否存在对应的md5
uint64_t check_file_md5_exist(task_t *pnew, MYSQL*conn,char *md5_str);

// file_system插入一条数据
uint64_t creat_file(task_t *pnew, MYSQL *conn, char* file_name, char *md5_str);

// 接收文件使用splice零拷贝技术
off_t recvFile(int new_fd, char* md5_file_name);

uint64_t creat_file_md5(MYSQL *conn, char *md5_str, off_t file_size);

// 获得文件名字并将其传入传出参数，并且返回文件已经接收的长度
off_t get_file_name_pos(char *file_name_recv_size);

// 获得文件的md5和文件大小
void get_md5_and_file_size(task_t *pnew, MYSQL *conn, char* md5_str, off_t *file_size, char *file_name_recv_size);

#endif