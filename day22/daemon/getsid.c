#include<func.h>

// 在前台运行时杀掉会话进程会导致进程被杀死， 而在后台进程运行则不会导致进程被杀死
int main(){
    int pid = fork();
    if(pid == 0){
        printf("I am child\n");
        printf("pid = %d, ppid = %d sid = %d\n", getpid(), getppid(), getsid(getpid()));
        while(1);
    }
    else{
        printf("I am parent\n");
        printf("pid = %d, ppid = %d sid = %d\n", getpid(), getppid(), getsid(pid));
        while(1);
    }
    return 0;
}