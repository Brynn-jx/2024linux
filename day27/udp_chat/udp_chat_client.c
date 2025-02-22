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
    // 发送消息给
    int ret = sendto(clifd, "begin!", 6, 0, (struct sockaddr*)&in_addr, len);
    ERROR_CHECK(ret, -1, "sendto"); 

    fd_set rdset;
    while (1){
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO, &rdset);
        FD_SET(clifd, &rdset);
        select(clifd + 1, &rdset, NULL, NULL, NULL);
        if(FD_ISSET(STDIN_FILENO, &rdset)){
            memset(buf, 0, sizeof(buf));
            ret = read(STDIN_FILENO, buf, sizeof(buf));
            if(ret == 0){
                break;
            }
            sendto(clifd, buf, strlen(buf) - 1, 0,  (struct sockaddr*)&in_addr, len);
        }   
        if(FD_ISSET(clifd, &rdset)){
            memset(buf, 0, sizeof(buf));
            ret = recvfrom(clifd, buf, sizeof(buf), 0, NULL, NULL);
            if(ret == 0){
                printf("bye bye\n");
                close(clifd);
                break;
            }
            printf("client: %s\n", buf);
        }
    }
    close(clifd);   
    return 0;
}