#include<func.h>

void sigfunc(int signum){
    time_t now;
    time(&now);
    printf("%s\n", ctime(&now));
}

int main(){
    signal(SIGPROF, sigfunc);
    struct itimerval timer;
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 3;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_PROF, &timer, NULL);   // 虚拟计时器会记录用户态模式下的CPU时间，当时间到的时候，会产生一个SIGPROF信号。
    sigfunc(0);
    sleep(1); // 会使程序进入内核态
    while(1);
    return 0;
}
