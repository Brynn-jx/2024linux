#include<func.h>

void *thread_fun(void* arg){
    printf("I am child thread\n");
    return (void*) 123;
}

int main(){
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, thread_fun, NULL);
    THREAD_ERR_CHECK(ret, "pthread_create");
    printf("I am main thread\n");
    long pret;
    pthread_join(tid, (void **)&pret);
    printf("child thread ret = %ld\n", pret);
    return 0;
}