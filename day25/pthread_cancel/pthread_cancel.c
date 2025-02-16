#include<func.h>

void *thread_fun(void* arg){
    printf("I am child thread\n");
    sleep(1);   // 为了子线程被cancel成功
    printf("You can not see me\n");
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