#include<func.h>

int main(){
    pid_t pid = fork();
    if(pid == 0){
        printf("I am child pid = %d ppid = %d\n", getpid(), getppid());
        return 0;
    }else{
        printf("I am parent pid = %d ppid = %d\n", getpid(), getppid());
        pid_t cpid;
        cpid = wait(NULL);
        printf("cpid = %d\n", cpid);
        return 0;
    }
    return 0;
}