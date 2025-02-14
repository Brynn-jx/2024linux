#include<func.h>

/**
接受到了另一个不同类型信号。那么当前的信号处理流程是会被中断的，CPU会先转移执行新到来的信号处理流程，执行完毕以后再恢复原来信号的处理流程。
接受到了另一个相同类型信号。那么当前的信号处理流程是会不会被中断的，CPU会继续原来的信号处理流程，执行完毕以后再响应新来到的信号。
如果接受到了连续重复的相同类型的信号，后面重复的信号会被忽略，从而该信号处理流程只能再多执行一次，因为是使用位图进行处理的。
 */
void sig_fun(int sig_num){
    printf(" Before %d signal is coming\n", sig_num);
    sleep(3);
    printf("After %d signal is coming\n", sig_num);
}

int main(){
    signal(SIGINT, sig_fun);
    signal(SIGQUIT, sig_fun);
    while(1);
    return 0;
}