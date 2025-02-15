#include<func.h>

void *threadFun(void * arg){
    printf("I am child thread %lu val = %ld\n", pthread_self(), (long)arg);
    return NULL;
}

// 使用ps -elfL 来查看线程 这里显示的线程id是使用的进程ID
int main(){
    pthread_t tid;
    long val = 1001;
    int ret = pthread_create(&tid, NULL, threadFun, (void *)val); // 虽然void* 看上去是传一个数据的地址，但是可以只传送一个整形数据，这时候
                                                                 // 就可以把他当做一个8字节的整形数据即可 
    THREAD_ERR_CHECK(ret, "create");
    val = 1002;
    printf("I am main thread %lu share = %ld\n", pthread_self(), val);
    usleep(10);
    return 0;
}