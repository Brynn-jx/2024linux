#include<func.h>

int main(){
    signal(SIGINT, SIG_IGN);
    sleep(10);
    signal(SIGINT, SIG_DFL); // 恢复之前的信号的功能
    while(1);
    return 0;
}