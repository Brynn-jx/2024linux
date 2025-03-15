#include"factory.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define LENGTH 19

// 随机生成盐值
void generate_random_string(char *random_string) {
    char *charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int i;

    srand(time(NULL)); // 设置种子
    
    for (i = 3; i < LENGTH; ++i) {
        random_string[i] = charset[rand() % (strlen(charset) - 1)];
    }

    random_string[LENGTH] = '\0'; // 添加字符串结尾
}

// 获得文件名字并将其传入传出参数，并且返回文件已经接收的长度
off_t get_file_name_pos(char *file_name_recv_size){
    int i;
    for(i = 0; i < strlen(file_name_recv_size); i++){
        if(file_name_recv_size[i] == ' '){
            break;
        }
    }
    file_name_recv_size[i] = 0;
    return atol(file_name_recv_size + i + 1);
}

