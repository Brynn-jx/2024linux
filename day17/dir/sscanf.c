#include<stdio.h>
#include<stdlib.h>

int main(){
    int a = 0;
    char *b = (char *)malloc(sizeof(char) * 10);    // 在使用字符指针接受字符串时别忘了要分配内存空间
    float c = 0;
    // 在读取字符串时遇到空格，逗号等等分隔符就会停止对字符串的匹配
    int ret = sscanf("17 abcd 75.60", "%d %s %f", &a, b, &c);
    if(ret == -1){
        perror("sscanf");
    }
    printf("%d %s %f\n", a, b, c);
    return 0;
}