#include<func.h>

const int N = 1000;
int main(){
    // 创建一个信号量
    int semid = semget(1000, 1, IPC_CREAT | 0600);
    ERROR_CHECK(semid, -1, "semget");
    int ret = semctl(semid, 0, SETVAL, 1);
    ERROR_CHECK(ret, -1, "semctl");
    struct sembuf P;
    P.sem_num = 0;  // 信号量的下标
    P.sem_op = -1;
    P.sem_flg = SEM_UNDO;

    struct sembuf V;
    V.sem_num = 0;  // 信号量的下标
    V.sem_op = 1;
    V.sem_flg = SEM_UNDO;

    int shmid = shmget(3000, 4096, 0600|IPC_CREAT);
    ERROR_CHECK(shmid, -1, "shmget");
    int *p = (int *)shmat(shmid, NULL, 0);
    p[0] = 0;
    if(!fork()){
        for (int i = 0; i < N; i++){
            semop(semid, &P, 1); // 1是信号量的数量
            p[0]++;
            semop(semid, &V, 1);
        }
        exit(0);
    }
    else{
        for (int i = 0; i < N; i++){
            semop(semid, &P, 1); // 1是信号量的数量
            p[0]++;
            semop(semid, &V, 1);
        }
        wait(NULL);
        printf("total = %d\n", p[0]);
    }
    return 0;
}


// #include <func.h>
// #define N 1000
// int main()
// {
//     int semArrid = semget(1000,1,IPC_CREAT|0600);
//     ERROR_CHECK(semArrid,-1,"semget");
//     int ret = semctl(semArrid,0,SETVAL,1);
//     ERROR_CHECK(ret,-1,"semctl");
//     struct sembuf P;
//     P.sem_num = 0;//信号量值在集合中的下标
//     P.sem_op = -1;//信号量值修改
//     P.sem_flg = SEM_UNDO;//如果P操作被终止，会自动恢复
//     struct sembuf V;
//     V.sem_num = 0;
//     V.sem_op = 1;
//     V.sem_flg = SEM_UNDO;
//     int shmid = shmget(1000,4096,IPC_CREAT|0600);
//     int *p = (int *)shmat(shmid,NULL,0);
//     p[0]=0;
//     if(!fork()){
//     for(int i =0; i < N; ++i){
//     semop(semArrid,&P,1);
//     ++p[0];
//     semop(semArrid,&V,1);
//     }
//     exit(0);
//     }
//     else{
//     for(int i =0; i < N; ++i){
//     semop(semArrid,&P,1);
//     ++p[0];
//     semop(semArrid,&V,1);
//     }
//     wait(NULL);
//     printf("p[0] = %d\n", p[0]);
// }
// return 0;
// }//如果程序的执行速度太慢，可以使用top检查进程执行状态