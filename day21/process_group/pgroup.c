#include <func.h>

// bash 进程是当前进程的父进程，按理说是属于bash组的，但是在创建该进程时在fork 和 exec中间使用了setpgid使其单独成为了一个组
// kill 加负的组号 就是杀死所有该进程组的进程
int main(){
    pid_t pid = fork();
    if(pid == 0){
        printf("child, pid = %d, pgid = %d\n", getpid(), getpgid(0));
        while(1);
        exit(0);
    }
    else{
        printf("parent, pid = %d, pgid = %d\n", getpid(), getpgid(0));
        wait(NULL);
        exit(0);
    }
}