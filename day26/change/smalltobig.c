#include<func.h>

/**
 * 内存中数据的存储是小端
 * 但是在网络中传输是大端方式，并且传输时是直接拿取内存中的数据
 * 所以必须将小端转化为大端存储
 */
int main(){
    unsigned short us = 0x1234;
    printf("%x\n", us);
    unsigned short us2 = htons(us);
    printf("%x\n", us2);
    return 0;
}