#include<stdio.h>
#include<sys/stat.h>
#include<stdlib.h>

int main(int argv, char* argc[]){
    if(argv != 3){
        perror("argv num error!");
        return -1;
    }
    char* c;
    unsigned int mode = atoi(argc[2]);
    unsigned long mode2 = strtol(argc[2], &c, 8);
    printf("%lu\n", mode2);
    int ret = chmod(argc[1], mode2);
    if(ret == -1){
        perror("chmod error");
        return -1;
    }
    return 0;
}