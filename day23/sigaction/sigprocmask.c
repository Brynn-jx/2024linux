#include<func.h>

void sig_fun(int sig_num){
    printf("Before %d is coming\n", sig_num);
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
    signal(SIGINT,sig_fun);
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    int ret = sigprocmask(SIG_BLOCK, &mask, NULL);  
    ERROR_CHECK(ret,-1,"sigprocmask");
    printf("block success!\n");
    sleep(3);    // 阻塞的是用户业务代码执行时的信号，而sa_mask是在执行自定义程序时进行的阻塞
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    return 0;
}