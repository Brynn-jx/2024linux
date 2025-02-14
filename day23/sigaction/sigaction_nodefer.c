#include<func.h>

void sig_fun(int sig_num, siginfo_t *p, void* p1){
    printf("Before %d is coming\n", sig_num);
    sleep(3);   // 注意信号可以提前终止sleep的时间 所以导致前面的信号被后面的信号终止了sleep的时间，
    // 只有最后一个信号无法终止，所以就会导致后面的东西一下子输出出来
    printf("After %d is coming\n", sig_num);
}

int main(){
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_flags = SA_SIGINFO; // 解除所有阻塞行为。特别地，执行信号处理流程可以处理同类信号传递，按照栈的方式执行,
                                // 无论发送几次信号都会执行。若是不加则就像是signal的处理方式
    act.sa_sigaction = sig_fun;
    int ret = sigaction(SIGINT, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);
    ERROR_CHECK(ret, -1, "sigaction");
    while(1);
    return 0;
}