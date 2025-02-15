#include<func.h>

void *threadFun(void * arg){
    printf("I am child thread %lu\n", pthread_self());
    return NULL;
}

// 使用ps -elfL 来查看线程 这里显示的线程id是使用的进程ID
int main(){
    pthread_t tid;
    struct timeval start, end;
    gettimeofday(&start,NULL);
    int ret = pthread_create(&tid, NULL, threadFun, NULL); // 第一个NULL是线程的属性，第二个NULL是线程函数指定的参数
    THREAD_ERR_CHECK(ret, "create");
    printf("I am main thread %lu\n", pthread_self());
    pthread_join(tid, NULL);
    gettimeofday(&end, NULL);
    printf("%ld\n", (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec));
    return 0;
}