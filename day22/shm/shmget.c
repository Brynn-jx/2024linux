#include<func.h>

int main(){
    // 创建一个共享内存
    int shmid = shmget(1000, 4096, 0600 | IPC_CREAT);   // 这个key是用来找到shmid的
    ERROR_CHECK(shmid, -1, "shmget");
    return 0;
}