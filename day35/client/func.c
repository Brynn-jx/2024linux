#include "head.h"
//请输入用户名和密码
int input_username_passwd(user_passwd_t *user_passwd){
    printf("username:\n");
    scanf("%s",user_passwd->username);
    printf("passwd:\n");
    scanf("%s",user_passwd->password);
    return 0;
}