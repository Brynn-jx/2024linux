#include<func.h>

int main(){
    pid_t pid = fork();
    if(pid == 0){
        printf("I am child\n");
    }else{
        printf("I am parent, child pid = %d\n", pid);
    }
    printf("父进程和子进程都会执行\n");
    return 0;
}