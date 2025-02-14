#include<func.h>

void sig_fun(int sig_num, siginfo_t *p, void* p1){
    printf(" %d is coming\n", sig_num);
}

int main(){
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_flags = SA_SIGINFO|SA_RESETHAND; // 执行一次就返回原来信号的作用
    act.sa_sigaction = sig_fun;
    int ret = sigaction(SIGINT, &act, NULL);
    ERROR_CHECK(ret, -1, "sigaction");
    while(1);
    return 0;
}