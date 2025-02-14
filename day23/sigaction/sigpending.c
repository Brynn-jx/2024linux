#include<func.h>

void sig_fun(int sig_num, siginfo_t *p, void* p1){
    printf("Before %d is coming\n", sig_num);
    sleep(3);   // 注意信号可以提前终止sleep的时间 所以导致前面的信号被后面的信号终止了sleep的时间，
    // 只有最后一个信号无法终止，所以就会导致后面的东西一下子输出出来
    sigset_t pendingSet;
    sigpending(&pendingSet);
    if(sigismember(&pendingSet, SIGQUIT)){
        printf("SIGQUIT is pending\n");
    }
   if(sigismember(&pendingSet, SIGINT)){
        printf("SIGINT is pending\n");
    }
    printf("After %d is coming\n", sig_num);
}

int main(){
    // 设置信号量
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = sig_fun;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGQUIT);   // 在执行2号信号时阻塞3号信号，阻塞是待会会执行，屏蔽是不会执行
    int ret = sigaction(SIGINT, &act, NULL);
    ERROR_CHECK(ret,-1,"sigaction");
    ret = sigaction(SIGQUIT,&act,NULL);
    ERROR_CHECK(ret,-1,"sigaction");
    while(1);
    return 0;
}