#include<func.h>

void sig_fun(int sig_num){
    
}

int main(){
    signal(SIGALRM, sig_fun);
    alarm(3); // 3秒后会发送一个SIGALARM信号 注意每个进程只有一个alarm信号
    pause();    // 会阻塞进程直到一个信号到来解除
    return 0;
}