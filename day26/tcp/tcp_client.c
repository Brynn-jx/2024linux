#include<func.h>

// 192.168.88.128 本地IP地址
// 客户端不需要使用bind
int main(int argc, char* argv[]){
    ARGS_CHECK(argc, 3);
    int socfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ERROR_CHECK(socfd, -1, "socket");
    // connect 如果不指定port会随即指定一个端口 用来完成三次握手
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