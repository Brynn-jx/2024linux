#include "head.h"

//socket,bind,listen的初始化
int tcp_init(char* ip,char* port,int *sfd)
{
    int sock_fd=socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sock_fd,-1,"socket");
    struct sockaddr_in serAddr;
    serAddr.sin_family=AF_INET;
    serAddr.sin_addr.s_addr=inet_addr(ip);
    serAddr.sin_port=htons(atoi(port));
    int ret;

    //地址重用
    int reuse=1;
    ret=setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
    ERROR_CHECK(ret,-1,"setsockopt");

    ret=bind(sock_fd,(struct sockaddr*)&serAddr,sizeof(serAddr));
    ERROR_CHECK(ret,-1,"bind");
    listen(sock_fd,10);
    *sfd=sock_fd;
    return 0;
}