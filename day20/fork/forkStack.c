#include<func.h>

int main(){
    int i = 0;
    pid_t p = fork();
    // 写时复制 虚拟地址相同物理地址不同
    if(p == 0){
        i++;
        printf("child pid = %d ppid = %d i = %d &i = %p\n", getpid(), getppid(), i, &i);
    }else{
        printf("parent pid = %d ppid = %d i = %d &i = %p\n", getpid(), getppid(), i, &i);
    }
    return 0;
}