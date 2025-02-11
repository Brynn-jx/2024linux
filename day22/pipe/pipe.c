#include<func.h>

int main(){
    int fd[2]; // 0读 1写
    pipe(fd); // 创建俩个匿名管道
    if(fork() != 0){
        close(fd[0]);
        write(fd[1], "hello world", 11);
        exit(0);
    }
    else{
        close(fd[1]);
        char buf[100];;
        read(fd[0], buf, 100);
        printf("%s\n", buf);
        wait(NULL);
        exit(0);
    }
    return 0;
}