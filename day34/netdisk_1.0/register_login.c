#include"factory.h"

int split_user_passwd(char* usrname_passwd,char* username,char* passwd){
    // 使用 strtok 函数来分割字符串
    char * token = strtok(usrname_passwd, " ");
    strcpy(username,token);
    token = strtok(NULL, " ");  // 这里设置为NULL表示从上一次的位置开始继续分 但这不是线程安全的后面可以改进
    strcpy(passwd,token);
    return 0;
}
