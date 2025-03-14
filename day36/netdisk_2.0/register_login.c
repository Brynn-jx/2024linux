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



