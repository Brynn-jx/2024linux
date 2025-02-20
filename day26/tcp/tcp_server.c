#include<func.h>

// 192.168.88.128 本地IP地址
// 服务器端调用 bind 是必不可少的，因为它明确了服务器的监听地址和端口，使得客户端能够通过这些信息找到并连接服务器。
// 相比之下，客户端通常不需要显式调用 bind，因为操作系统会自动为其分配一个临时端口。
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 3);
    // 创建socket设备 在内核会有对应的接受发送缓冲区
    int socfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ERROR_CHECK(socfd, -1, "socket");
    // bind函数用于给套接字赋予一个本地协议地址（即IP地址加端口号）。
    struct sockaddr_in in_addr;
    memset(&in_addr, 0, sizeof(in_addr));
    in_addr.sin_family = AF_INET;
    in_addr.sin_addr.s_addr = inet_addr(argv[1]);   // 将字符串的点分十进制转为二进制
    in_addr.sin_port = htons(atoi(argv[2]));
    int ret = bind(socfd, (struct sockaddr*)&in_addr, sizeof(in_addr));
    ERROR_CHECK(ret,-1,"bind");
    //listen 使TCP服务端开启监听。服务端在开启了 listen 之后，就可以开始接受客户端连接了。
    ret = listen(socfd, 10);  // 能够监听10个
    ERROR_CHECK(ret, -1, "lsiten");
    // accept
    socklen_t len;
    len = sizeof(struct sockaddr);
    int newFD = accept(socfd, NULL, &len);
    ERROR_CHECK(newFD, -1, "accept");
    // recv
    char buf[100] = {0};
    fd_set rdset;
    while (1){
        FD_ZERO(&rdset);
        FD_SET(newFD, &rdset);
        FD_SET(STDIN_FILENO, &rdset);
        select(newFD + 1, &rdset, NULL, NULL, NULL);
        if(FD_ISSET(STDIN_FILENO, &rdset)){
            memset(buf, 0, sizeof(buf));
            int ret = read(STDIN_FILENO, buf, sizeof(buf));
            if(ret==0){
                break;
            }
            send(newFD, buf, strlen(buf) - 1, 0); // 如果可以标准读就是发送
        }
        else if(FD_ISSET(newFD, &rdset)){
            memset(buf, 0, sizeof(buf));
            int ret = read(newFD, buf, sizeof(buf));
            if(0 == ret){
                printf("byebye\n");
                close(socfd);
                close(newFD);
                return 0;
            }
            printf("buf=%s\n",buf);
        }
    }
    
    close(newFD);
    close(socfd);
    return 0;
}