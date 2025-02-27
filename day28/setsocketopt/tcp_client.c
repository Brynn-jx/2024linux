#include<func.h>
// client
// 192.168.88.128 本地IP地址
// 客户端不需要使用bind 这里connect默认给生成一个端口和本机ip绑定
int main(int argc, char* argv[]){
    ARGS_CHECK(argc, 3);
    // 这里创建的套接字初始化通信接口
    int socfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ERROR_CHECK(socfd, -1, "socket");
    // connect 向服务器发起连接请求并完成TCP三次握手并且明确指定服务器端ip地址和端口号 
    // connect() 本身并没有直接负责分配端口或绑定IP地址的任务，但它触发了操作系统内部机制去完成这些必要的步骤以建立成功的TCP连接
    // 操作系统内核会为客户端的套接字自动分配一个本地 IP 地址和一个临时端口号（动态端口），以便与服务器建立连接
    struct sockaddr_in in_addr;
    in_addr.sin_family = AF_INET;
    in_addr.sin_addr.s_addr = inet_addr(argv[1]);
    in_addr.sin_port = htons(atoi(argv[2]));    // h = host n = network s = short(ipv4)16bit l = long(ipv6)64bit 
    int ret = connect(socfd, (struct sockaddr*)&in_addr, sizeof(in_addr));
    ERROR_CHECK(ret, -1, "connect");
    // send
    char buf[100];
    fd_set rdset;
    while (1){
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(socfd, &rdset);
        select(socfd + 1, &rdset, NULL, NULL, NULL);
        if(FD_ISSET(STDIN_FILENO, &rdset)){
            memset(buf, 0, sizeof(buf));
            int ret = read(STDIN_FILENO, buf, sizeof(buf));
            if(ret == 0){
                break;
            }
            send(socfd, buf, strlen(buf) - 1, 0);
        }
        else if(FD_ISSET(socfd, &rdset)){
            memset(buf, 0, sizeof(buf));
            int ret = recv(socfd, buf, sizeof(buf), 0);
            // select()会持续检测socfd是否可读。如果服务器已经关闭连接，socfd仍然会被标记为可读，因为操作系统会将已关闭连接的状态通知给客户端
            // 当recv()返回值为0时，表示连接已被对端关闭，客户端应该关闭套接字并退出程序
            if(ret == 0){
                printf("byebye\n");
                close(socfd);
                return 0;
            }
            printf("buf=%s\n",buf);
        }
    }
    
    return 0;
}