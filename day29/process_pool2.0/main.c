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
    printf("epfd %d\n", epfd);

    // 注册监控子进程和父进程相关联的套接字端口
    for(int i = 0; i < processNum; i++){
        epoll_add(epfd, dataList[i].pipeFd);
    }

    int ready_fd_num, new_fd;
    struct epoll_event *evs=(struct epoll_event *)calloc(processNum + 1,
        sizeof(struct epoll_event));
    
    struct sockaddr_in in_addr;
    int addr_len;

    char buf[30];
    while(1){
        ready_fd_num = epoll_wait(epfd, evs, processNum + 1, -1);
        
        for(int i = 0; i < ready_fd_num; i++){
            if(evs[i].data.fd == sockFd){   // 客户端发来请求
                addr_len = sizeof(in_addr);
                new_fd = accept(sockFd, (struct sockaddr *)&in_addr, &addr_len);
                printf("task coming ip=%s,port=%d\n",inet_ntoa(in_addr.sin_addr),
                    ntohs(in_addr.sin_port));
                // 寻找空闲的子进程将任务传给他
                for(int j = 0; j < processNum; j++){
                    if(dataList[i].status == FREE){
                        sendFd(dataList[i].pipeFd, new_fd);
                        dataList[j].status=BUSY;//把其标记为busy
                        printf("%d 派给 %d 子进程\n",new_fd,dataList[j].pid);
                        break;
                    }
                }
                close(new_fd); // 在sendFd时会使new_fd引用计数加1，父进程关闭一次，子进程再关闭一次才会进行四次挥手
            }
            else{   // 子进程完成任务了
                for(int j = 0; j < processNum; j++){
                    if(evs[i].data.fd == dataList[j].pipeFd){
                        // 读取子进程发送过来的信息
                        read(dataList[j].pipeFd, buf, sizeof(buf));
                        printf("%d finish task %s\n",dataList[j].pid, buf);
                        dataList[j].status = FREE;
                        break;
                    }
                }
            }
        }
    }
    return 0;
}