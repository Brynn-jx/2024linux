#include<func.h>

void *thread_fun(void *p){
    time_t now;
    time(&now);
    char *t = ctime(&now);
    printf("%s\n", t);
    sleep(5);
    printf("%s\n", t);
    pthread_exit(NULL);
}
// 线程不安全 由于多线程之间是共享同一个进程地址空间，所以多线程在访问共享数据的时候会出现竞争问题
// ctime会把时间存储在静态区域导致出现问题，使用ctime_r就可以避免，因为会把时间存储在线程自己申请的空间
// 信号、多线程这些情况下，一个函数在执行过程中，有可能会异步地再重新调用同一个函数。如果重复的函数调用有可能会导致错乱的结果，那么这些函数就是不可重入的。
// 全局变量，静态，都会导致不可重入
int main(){
    pthread_t tid;
    pthread_create(&tid, NULL, thread_fun, NULL);
    printf("I am main thread\n");
    sleep(2);
    time_t now;
    time(&now);
    printf("main time = %s\n",ctime(&now));
    long pret;
    int ret = pthread_join(tid, (void **)&pret);
    THREAD_ERR_CHECK(ret, "pthread_join");
    return 0;
}