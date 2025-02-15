#include<func.h>

void *threadFun(void * arg){
    int *tmp = (int *)arg;
    *tmp = 1002;
    printf("I am child thread %lu val = %d\n", pthread_self(), *tmp);
    return NULL;
}

// 线程之间虽然是栈之间是独立的但是各个线程之间是可以互相访问栈空间的
int main(){
    pthread_t tid;
    int val = 1001;
    int ret = pthread_create(&tid, NULL, threadFun, (void *)&val); 
    THREAD_ERR_CHECK(ret, "create");
    sleep(1);
    printf("I am main thread %lu share = %d\n", pthread_self(), val);
    return 0;
}