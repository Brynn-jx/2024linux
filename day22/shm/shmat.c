#include<func.h>

int main(){
    // 创建一个共享内存
    int shmid = shmget(1000, 4096, 0600 | IPC_CREAT);   // 第二次不会创建共享内存而是用这个key是用来找到shmid的
    ERROR_CHECK(shmid, -1, "shmget");
    char *p = shmat(shmid, NULL, 0);    // 链接共享内存
    ERROR_CHECK(p, (char*)-1, "shmat");
    while (1);
    return 0;
}