#include<func.h>

int main(){
    pid_t pid = fork();
    if(pid == 0){
        printf("I am child pid = %d ppid = %d", getpid(), getppid());   // 这里使用_exit不会刷新缓冲区所以不显示这句话 使用\n或fflush(stdout)刷新缓冲区
        _exit(1);    //终止子进程
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