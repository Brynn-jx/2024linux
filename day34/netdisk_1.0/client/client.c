#include "head.h"

int recvFile(int new_fd);

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
    bzero(&user_passwd, sizeof(user_passwd_t));
    char user_plus_password[100] = {0};
    int control_code;
    printf("1.注册, 2.登录\n");
    scanf("%d", &control_code);
    input_username_passwd(user_passwd.username, user_passwd.password);
    sprintf(user_plus_password, "%s %s", user_passwd.username, user_passwd.password);    
    if(control_code == 1){
        send_protocol(client_fd, 1, user_plus_password);
    }else if(control_code == 2){
        send_protocol(client_fd, 2, user_plus_password);
    }else{
        printf("非法输入!\n");
        exit(0);
    }
    while(1);
    //连接上以后就接文件
    recvFile(client_fd);
    close(client_fd);
    return 0;
}