#include<func.h>

int main(){
    pid_t pid = fork();
    if(pid == 0){
        printf("child pid = %d\n", getpid());
        while(1);
    }else{
        printf("parent pid = %d\n", getpid());
    }
    return 0;
}