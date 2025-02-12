#include<func.h>

// 相较于第一段代码好处主要是进行了信号量的同步，这样就不会导致没有产品也能买到产品，没有空间也能产出产品
int main(){
    // 创建信号量
    int semArrid = semget(1000, 2, 0600|IPC_CREAT);
    ERROR_CHECK(semArrid, -1, "semget");
    unsigned short Arr[2] = {0, 10};
    semctl(semArrid, 0, SETALL, Arr);
    if(!fork()){
        //子进程是消费者
        struct sembuf consume[2];
        consume[0].sem_num = 0; //0表示商品 1表示空格
        consume[0].sem_op = -1;//商品减1
        consume[0].sem_flg = SEM_UNDO;
        consume[1].sem_num = 1;
        consume[1].sem_op = 1;//空格加1
        consume[1].sem_flg = SEM_UNDO;
        while(1){
            sleep(2);
            printf("I am consumer, before consume, product = %d, space = %d\n",
            semctl(semArrid,0,GETVAL),semctl(semArrid,1,GETVAL));
            //semop(semArrid,consume,2); 这种写法显示太快了
            semop(semArrid,&consume[0],1);
            printf("Buy a product\n");
            semop(semArrid,&consume[1],1);
            printf("I am consumer, after consume, product = %d, space = %d\n",
            semctl(semArrid,0,GETVAL),semctl(semArrid,1,GETVAL));
        }
        exit(0);
        }
        else{
            //父进程是生产者
            struct sembuf produce[2];
            produce[0].sem_num = 0; //0表示商品 1表示空格
            produce[0].sem_op = 1;//商品加1
            produce[0].sem_flg = SEM_UNDO;
            produce[1].sem_num = 1;
            produce[1].sem_op = -1;//空格减1
            produce[1].sem_flg = SEM_UNDO;
            while(1){
                sleep(1);
                printf("I am producer, before produce, product = %d, space = %d\n",
                semctl(semArrid,0,GETVAL),semctl(semArrid,1,GETVAL));
                //semop(semArrid,produce,2);
                semop(semArrid,&produce[1],1);
                printf("Produce a product\n");
                semop(semArrid,&produce[0],1);
                printf("I am producer, after produce, product = %d, space = %d\n",
                semctl(semArrid,0,GETVAL),semctl(semArrid,1,GETVAL));
            }
            wait(NULL);
        }

    return 0;
}



