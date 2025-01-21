#include<func.h>

void print1(){
    printf("这是注册进程1\n");
}

void print2(){
    printf("这是注册进程2\n");
}

int main(){
    pid_t pid = fork();
    if(pid == 0){
        printf("I am child pid = %d ppid = %d\n", getpid(), getppid());
        atexit(print1);
        atexit(print2);
        exit(1);    //终止子进程
    }else{
        printf("I am parent pid = %d ppid = %d\n", getpid(), getppid());
        pid_t cpid;
        cpid = wait(NULL);
        exit(2);    // 终止父进程
        printf("cpid = %d\n", cpid);    //不会打印这句话
        return 0;
    }
    return 0;
}