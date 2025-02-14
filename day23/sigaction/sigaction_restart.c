#include<func.h>

void sig_fun(int sig_num, siginfo_t *p, void* p1){
    printf(" %d is coming\n", sig_num);
}

int main(){
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_flags = SA_SIGINFO|SA_RESTART; // 可以重启杯中断的系统调用和signal一样， 默认是系统调用出错并终止，
    act.sa_sigaction = sig_fun;
    int ret = sigaction(SIGINT, &act, NULL);
    ERROR_CHECK(ret, -1, "sigaction");
    char buf[1024] = {0};
    ret = read(STDIN_FILENO, buf, sizeof(buf));
    ERROR_CHECK(ret, -1, "read");
    return 0;
}