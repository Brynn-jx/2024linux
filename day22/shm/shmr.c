#include<func.h>

int main(){
    // 创建一个共享内存
    int shmid = shmget(1000, 4096, 0600 | IPC_CREAT);   // 这个key是用来找到shmid的
    ERROR_CHECK(shmid, -1, "shmget");
    // 获取到映射内存的首地址 进程终止不会使连接数加1
    char *p = (char *)shmat(shmid, NULL, 0);
    // 读出共享内存中的内容
    puts(p);
    return 0;
}

