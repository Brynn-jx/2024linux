#include<func.h>

void *thread_fun(void *p){
    pthread_exit(NULL);
}

int main(){
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);    // 设置这个属性就代表不能进行join，否则会出错
    pthread_create(&tid, &attr, thread_fun, NULL);
    printf("I am main thread\n");
    long pret;
    int ret = pthread_join(tid, (void **)&pret);
    THREAD_ERR_CHECK(ret, "pthread_join");
    return 0;
}