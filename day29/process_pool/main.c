#include"head.h"

int main(int argc, char *argv[]){   
    ARGS_CHECK(argc, 4);
    int processNum = atoi(argv[3]);
    processData_t *dataList = (processData_t*)calloc(sizeof(processData_t), processNum);
    makeChild(dataList, processNum); //创建好子进程
    
    // 服务端接受客户端发送的请求并且为客户端分配空闲的子进程进行文件传输

    // 将接受到的客户端的文件描述符发送给子进程
    int sockFd;
    tcp_init(argv[1], argv[2], &sockFd);
    //注册监控sock_fd
    int epfd=epoll_create(1);
    epoll_add(epfd, sockFd);

    // 注册监控子进程和父进程相关联的套接字端口
    for(int i = 0; i < processNum; i++){
        epoll_add(epfd, dataList[i].pipeFd);
    }

    int ready_fd_num, new_fd;
    struct epoll_event *evs=(struct epoll_event *)calloc(processNum + 1,
        sizeof(struct epoll_event));

    while(1){
        ready_fd_num = epoll_wait(epfd, evs, processNum + 1, -1);
        for(int i = 0; i < ready_fd_num; i++){
            
        }
    }
    return 0;
}