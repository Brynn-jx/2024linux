#include<func.h>

int main(int argc, char *argv[]){
    ARGS_CHECK(argc, 3);
    int clifd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    ERROR_CHECK(clifd, -1, "socket");
    struct sockaddr_in in_addr;
    in_addr.sin_family = AF_INET;
    in_addr.sin_addr.s_addr = inet_addr(argv[1]);
    in_addr.sin_port = htons(atoi(argv[2]));

    int len = sizeof(in_addr);
    char buf[100] = {0};
    int ret = sendto(clifd, "hello world", 11, 0, (struct sockaddr*)&in_addr, len);
    ERROR_CHECK(ret, -1, "sendto");
    return 0;
}