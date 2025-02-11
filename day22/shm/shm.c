#include<func.h>

const int MAX = 1000000;
int main(){
    int shmid = shmget(2000, 4096, 0600|IPC_CREAT);
    ERROR_CHECK(shmid, -1, "shmget");
    int *p = (int *)shmat(shmid, NULL, 0);
    p[0] = 0;
    pid_t pid = fork();
    if(pid == 0){
        for(int i = 0; i < MAX; i++)
            p[0]++;
        exit(0);
    }
    else{
        for (int i = 0; i < MAX; i++){
            p[0]++;
        }
        wait(NULL);
        printf("total=%d\n", p[0]);
        exit(0);
    }
    return 0;
}