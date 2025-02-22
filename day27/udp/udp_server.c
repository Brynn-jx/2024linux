#include<func.h>

// 192.168.88.128
int main(int argc, char *argv[]){
    ARGS_CHECK(argc, 3);
// udp创建的socket缓冲区只有一个缓冲区，用来接收发送的数据，写的数据直接丢给了路由器
    int serfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    ERROR_CHECK(serfd, -1, "socket");
    struct sockaddr_in in_addr;
    memset(&in_addr, 0, sizeof(in_addr));
    in_addr.sin_family = AF_INET;
    in_addr.sin_addr.s_addr = inet_addr(argv[1]);
    in_addr.sin_port = htons(atoi(argv[2]));
    int ret = bind(serfd, (struct  sockaddr*)&in_addr, sizeof(in_addr));
    ERROR_CHECK(ret, -1, "bind");

    struct  sockaddr_in client;
    int len = sizeof(client);
    char buf[100] = {0};
    // 这里的client用来接收客户端的地址信息，方便之后给客户端发送信息
    // 只能读取一次，缓冲区中有数据也会被丢弃
    recvfrom(serfd, buf, sizeof(buf), 0, (struct sockaddr*)&client, &len);
    printf("recvfrom %s\n", buf);
    return 0;
}