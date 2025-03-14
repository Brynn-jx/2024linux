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
        recv_protocol(client_fd, &control_code, NULL);
        if(control_code == LOGIN_SUCCESS){
            printf("登陆成功\n");
        }
        else{
            goto start;
        }
        
    }else{
        printf("非法输入!\n");
        goto start;
    }
    // 处理登陆后的事件
    getchar(); // 去除换行
    // 这里是登录了,开始发各种请求
    char cmd[1000];
    char path[1000]; // 各个命令后面带的参数存储
    while (1){
        fgets(cmd, sizeof(cmd), stdin);
        printf("cmd=%s\n", cmd);
        cmd[strlen(cmd) - 1] = 0; // 去除\n
        // 格式化字符串，将命令以空格分开
        char *p = split_space(cmd);
        strcpy(cmd, p);
        printf("%s\n", cmd);
        system("clear");
        if(strncmp(cmd, "ls", 2) == 0){
            send_protocol(client_fd, LS, NULL, 0);
            int recvLen;
            char buf[1024];
            while(1){
                recvn(client_fd, &recvLen, 4);
                if(recvLen){
                    bzero(buf, sizeof(buf));
                    recvn(client_fd, buf, recvLen);
                    puts(buf);
                }
                else
                    break;
            }
        }
        else if(strncmp(cmd, "cd", 2) == 0){
            if(strlen(cmd) <= 3)
                continue;
            send_protocol(client_fd, CD, cmd + 3, strlen(cmd) - 3);
            bzero(path, sizeof(path));
            recv_protocol(client_fd, &control_code, path);
            if(control_code == SUCCESS){
                printf("%s\n", path);
            }
            else{
                printf("路径错误\n"); 
            }
        }
        else if(strncmp(cmd, "pwd", 3) == 0){
            send_protocol(client_fd, PWD, cmd, strlen(cmd));
            bzero(path, sizeof(path));
            recv_protocol(client_fd, &control_code, path);
            printf("%s\n", path); // 输出path
        }
        else if(strncmp(cmd, "mkdir", 5) == 0){
            if(strlen(cmd) <= 6)
                continue;
            send_protocol(client_fd, MKDIR, cmd + 6, strlen(cmd) - 6);
            recv_protocol(client_fd, &control_code, NULL);
            if(control_code == SUCCESS){
                printf("创建成功\n");
            }
            else{
                printf("创建失败\n");
            }
        }
        else if(strncmp(cmd, "rm", 2) == 0){
            if(strlen(cmd) <= 3)
                continue;
            send_protocol(client_fd, RM, cmd + 3, strlen(cmd) - 3);
            recv_protocol(client_fd, &control_code, NULL);
            if(control_code == SUCCESS){
                printf("删除成功\n");
            }
            else{
                printf("删除失败\n");
            }
        }
        else if(strncmp(cmd, "gets", 4) == 0){
            
        }
        
        else if(strncmp(cmd, "puts", 4) == 0){
            
        }
    }
    
    //连接上以后就接文件
    recvFile(client_fd);
    close(client_fd);
    return 0;
}