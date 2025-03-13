#include "head.h"

int main(int argc,char *argv[]){
    ARGS_CHECK(argc,3);
    int client_fd=socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(client_fd,-1,"socket");
    struct sockaddr_in serAddr;
    serAddr.sin_family=AF_INET;
    serAddr.sin_addr.s_addr=inet_addr(argv[1]);
    serAddr.sin_port=htons(atoi(argv[2]));
    int ret;
    ret=connect(client_fd,(struct sockaddr*)&serAddr,sizeof(serAddr));
    ERROR_CHECK(ret,-1,"connect");

    // 注册登录模块
    user_passwd_t user_passwd;
start:
    bzero(&user_passwd, sizeof(user_passwd_t));
    int control_code;
    printf("1.注册, 2.登录\n");
    scanf("%d", &control_code);
    if(control_code == 1){
        input_username_passwd(&user_passwd); 
        send_protocol(client_fd, control_code, &user_passwd, sizeof(user_passwd));
        recv_protocol(client_fd, &control_code, NULL);
        if(control_code == REGISTER_FAILED){
            goto start;
        }
        printf("注册成功，已登录\n");
    }else if(control_code == 2){
        input_username_passwd(&user_passwd); 
        send_protocol(client_fd, control_code, &user_passwd, sizeof(user_passwd));
        printf("发送成功\n");
    }else{
        printf("非法输入!\n");
        goto start;
    }
    while(1);
    //连接上以后就接文件
    recvFile(client_fd);
    close(client_fd);
    return 0;
}