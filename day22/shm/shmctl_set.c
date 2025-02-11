#include<func.h>

int main(){
    // 创建一个共享内存
    int shmid = shmget(2000, 4096, 0600 | IPC_CREAT);   // 这个key是用来找到shmid的
    ERROR_CHECK(shmid, -1, "shmget");
    // 连接
    shmat(shmid, NULL, 0);
    // 获取共享内存段的信息
    struct shmid_ds stat;
    int ret = shmctl(shmid, IPC_STAT, &stat);
    ERROR_CHECK(ret, -1, "shmctl");
    printf("cuid = %d perm = %o size= %ld nattch = %ld\n",stat.shm_perm.cuid,stat.shm_perm.mode,stat.shm_segsz,stat.shm_nattch);
    stat.shm_perm.mode = 0666;
    ret = shmctl(shmid, IPC_SET, &stat);
    printf("cuid = %d perm = %o size= %ld nattch = %ld\n",stat.shm_perm.cuid,stat.shm_perm.mode,stat.shm_segsz,stat.shm_nattch);
    return 0;
}