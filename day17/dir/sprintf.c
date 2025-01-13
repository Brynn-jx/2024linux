#include<stdio.h>

int main(){
    char str[1024] = {0};
    int a = 17;
    char* b = "abcd";
    float c = 75.6;
    // sprintf 就是将字符串拼接起来
    sprintf(str, "a = %d b = %s c = %.2f", a, b, c);
    puts(str);
    return 0;
}