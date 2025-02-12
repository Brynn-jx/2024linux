#include<func.h>

int main(){
    int semArrid = semget(1000, 1, 0600|IPC_CREAT);
    ERROR_CHECK(semArrid, -1, "semget");
    semctl(semArrid, 0, SETVAL, 5);
    int val = semctl(semArrid, 0, GETVAL);
    ERROR_CHECK(val, -1, "semctl");
    printf("%d\n", val);
    semctl(semArrid,0,IPC_RMID);
    return 0;
}