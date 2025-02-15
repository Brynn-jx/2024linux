#include<func.h>

void *threadFun(void * arg){
    char *pHeap = (char *)arg;
    strcpy(pHeap, "world");
    printf("I am child thread %s\n", pHeap);
    return NULL;
}

// 使用ps -elfL 来查看线程 这里显示的线程id是使用的进程ID
int main(){
    pthread_t tid;
    char *p = (char *)malloc(20);
    strcpy(p, "hello");
    int ret = pthread_create(&tid, NULL, threadFun, (void *)p); // 第一个NULL是线程的属性，第二个NULL是线程函数指定的参数
    THREAD_ERR_CHECK(ret, "create");
    sleep(1);
    printf("I am main thread %s\n", p);
    free(p);
    return 0;
}