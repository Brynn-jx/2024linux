#ifndef __HEAD_H__
#define __HEAD_H__
#define _GNU_SOURCE
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>
#include <strings.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <syslog.h>
#include <sys/sendfile.h>
#define ARGS_CHECK(argc,num) {if(argc!=num) {printf("error args\n");return -1;}}
#define ERROR_CHECK(ret,retval,func_name) {if(ret==retval) {perror(func_name);printf("filename=%s,line=%d\n",__FILE__,__LINE__);return -1;}}
#define THREAD_ERR_CHECK(ret,func_name) {if(ret!=0) {printf("%s failed,%d %s\n",func_name,ret,strerror(ret));return -1;}}

//这个结构体两个成员位置不可以交换
typedef struct{
    int size;//火车头，代表火车车厢上放了多少数据
    char buf[1000];//任何数据都可以装
} train_t;

// 协议火车，用于接收除了发送文件之外的所有操作
typedef struct{
    int size;   // 发送字符串的长度
    int control_code;   // 控制码，客户端和服务端集成在一起使用
    char buf[1000];
}protocol_t;

typedef struct{
    char username[50];
    char password[50];
}user_passwd_t;

int input_username_passwd(char *username,char* passwd);

//buf里只能是字符串,发数据
int send_protocol(int new_fd,int control_code,char *pdata);

//buf里只能是字符串，接数据
int recv_protocol(int new_fd,int *control_code,char *pdata);

#endif
