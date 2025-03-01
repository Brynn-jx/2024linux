#include<func.h>

enum workStatus{
    FREE,
    BUSY
};

typedef struct {
    pid_t pid;
    int status;
    int pipeFd;
}processData_t;

// 在应用层协议上构建一个私有协议 这两个元素不可以交换位置，因为网络传输的是字节流
typedef struct {
    int size;
    char buf[1000];
}train_t;


int makeChild(processData_t *dataList, int processNum);

void handleEvent(int pipeFd);

//socket,bind,listen的初始化
int tcp_init(char* ip,char* port,int *sock_fd);

int epoll_add(int epfd, int fd);

// 通过本地的TCP套接字发送文件描述符给子进程
int sendFd(int pipeFd, int fd);

// 接收文件描述符
int recvFd(int pipeFd, int* fd);