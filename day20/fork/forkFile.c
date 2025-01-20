//forkFile类似于文件的dup复制文件的引用计数加1，共享文件的指针
#include <func.h>
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
        read(fd,buf,5);
        printf("I am parent process, buf = %s\n", buf);
    }
}