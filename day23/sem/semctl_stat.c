#include <func.h>
int main()
{
    int semArrid = semget(1000,2,IPC_CREAT|0600);
    ERROR_CHECK(semArrid,-1,"semget");
    struct semid_ds stat;
    semctl(semArrid,0,IPC_STAT,&stat);
    printf("cuid = %d perm = %o nsems=%ld\n",stat.sem_perm.cuid,stat.sem_perm.mode,stat.sem_nsems);
    stat.sem_perm.mode = 0666;
    semctl(semArrid,0,IPC_SET,&stat);
    printf("cuid = %d perm = %o nsems=%ld\n",stat.sem_perm.cuid,stat.sem_perm.mode,stat.sem_nsems);
    return 0;
}