#include<func.h>

void *thread_fun(void* arg){
    while(1){
        pthread_testcancel(); // 设置取消点 检查取消标志位
    };
    return (void*) 1;
}
// 因为线程之间的信号量是共享的所以不可以使用信号量来终结线程，所以要一种在用户态的取消方式
int main(){
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, thread_fun, NULL);
    THREAD_ERR_CHECK(ret, "pthread_create");
    printf("I am main thread\n");
    pthread_cancel(tid);    // 将子线程中的取消标志位设置为1，当子线程执行到取消点时就会取消线程
    long pret;
    pthread_join(tid, (void **)&pret);
    printf("child thread ret = %ld\n", pret);   // 若是线程被取消得到的返回值是-1
    return 0;
}