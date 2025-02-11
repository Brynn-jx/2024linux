#include<func.h>

int main(){
    // 创建一个共享内存    
    int shmid = shmget(1000, 4096, 0600 | IPC_CREAT);   // 这个key是用来找到shmid的
    ERROR_CHECK(shmid,-1, "shmget");
    // 获取到映射内存的首地址
    char *p = (char *)shmat(shmid, NULL, 0);
    ERROR_CHECK(p,(char *)-1,"shmat");
    // 使用strcopy进行写入
    strcpy(p, "hello world");
    return 0;
}
