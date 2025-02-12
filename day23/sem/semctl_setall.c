#include<func.h>

int main(){
    int semArrid = semget(1000, 2, 0600|IPC_CREAT);
    ERROR_CHECK(semArrid, -1, "semget");
    unsigned short arr[2] = {3, 5};
    int ret = semctl(semArrid, 0, SETALL, arr);
    ERROR_CHECK(ret, -1, "semctl");
    unsigned short arr2[2];
    int val = semctl(semArrid, 0, GETALL, arr2);
    ERROR_CHECK(val, -1, "semctl");
    printf("%d %d\n", arr2[0], arr2[1]);
    semctl(semArrid,0,IPC_RMID);
    return 0;
}