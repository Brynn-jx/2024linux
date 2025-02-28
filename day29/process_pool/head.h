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

int makeChild(processData_t *dataList, int processNum);

void handleEvent(int pipeFd);

//socket,bind,listen的初始化
int tcp_init(char* ip,char* port,int *sock_fd);

int epoll_add(int epfd, int fd);