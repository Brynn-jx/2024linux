#include<func.h>

int main(){
    pid_t pid = fork();
    if(pid == 0){
        char *p = NULL;
        *p = 'a';
        printf("I am child pid = %d ppid = %d\n", getpid(), getppid());
        return 0;
    }else{
        printf("I am parent pid = %d ppid = %d\n", getpid(), getppid());
        pid_t cpid;
        int sign;
        cpid = waitpid(pid, &sign, 0);
        if(WIFEXITED(sign)){
            printf("cpid = %d sign = %d 正确退出\n", cpid, WEXITSTATUS(sign));
        }else{
            printf("cpid = %d sign = %d 错误退出\n", cpid, WTERMSIG(sign));
        }
        return 0;
    }
    return 0;
}