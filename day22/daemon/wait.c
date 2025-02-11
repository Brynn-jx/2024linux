#include<func.h>

int main(){
    int pid = fork();
    if(pid == 0){
        printf("I am child\n");
        printf("pid = %d, ppid = %d pgid = %d\n", getpid(), getppid(), getpgid(0));
        // 第一个0代表是对本进程的修改 第二个0是代表将该进程独立为一个进程组
        setpgid(0, 0);
        printf("pid = %d, ppid = %d pgid = %d\n", getpid(), getppid(), getpgid(0));
    }
    else{
        printf("I am parent\n");
        printf("pid = %d, ppid = %d pgid = %d\n", getpid(), getppid(), getpgid(0));
        int status = 0;
        wait(&status);
        if(WIFEXITED(status)){
            printf("回收成功， %d\n",WEXITSTATUS(status));
        }
       exit(0);
    }
    return 0;
}