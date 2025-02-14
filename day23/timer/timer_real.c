#include<func.h>

void sigfunc(int signum){
    time_t now;
    time(&now);
    printf("%s\n", ctime(&now));
}

int main(){
    signal(SIGALRM, sigfunc);
    struct itimerval timer;
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 3;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);   // 真实计时器会记录真实的时间（也就是时钟时间），当时间到时，会产生一个SIGALRM信号。
    while(1);
    return 0;
}