#include "head.h"

//注册监控某个描述符是否可读
int epoll_add(int epfd,int fd)
{
    struct epoll_event event;
    event.data.fd=fd;
    event.events=EPOLLIN;
    int ret=epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl");
    return 0;
}