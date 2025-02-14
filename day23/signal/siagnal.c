#include<func.h>

void sig_fun(int sig_num){
    printf("signal is coming\n");
}

int main(){
    signal(SIGINT, sig_fun);
    while(1);
    return 0;
}