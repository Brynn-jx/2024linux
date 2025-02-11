#include<func.h>

int main(){
    // 创建私有的共享内存 只能在亲缘关系下进行内存共享
    int shmid = shmget(IPC_PRIVATE, 4096, 0600|IPC_CREAT);
    char* p = (char*)shmat(shmid, NULL, 0);
    if(fork() != 0){
        strcpy(p, "hello world");
        shmdt(p);
        exit(0);
    }else{
        wait(NULL);
        puts(p);
        shmdt(p);
        exit(0);
    }
    return 0;
}