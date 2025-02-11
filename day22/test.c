#include<func.h>

// fork在处理文件时和dup一样共享文件位置指针
int main(){
    int fd = open("file", O_RDWR);//特别注意，文件打开要在fork之前。
    ERROR_CHECK(fd,-1,"open");
    pid_t pid = fork();
    if(pid == 0){
        printf("I am child process\n");
        //lseek(fd,5,SEEK_SET);
        write(fd,"hello",5);
    }
    else{
        printf("I am parent process\n");
        sleep(1);
        char buf[6] = {0};
        read(fd, buf, 5);
        printf("I am parent process, buf = %s\n", buf);
    }
    return 0;
}