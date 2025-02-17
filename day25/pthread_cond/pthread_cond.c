#include<func.h>
/*
它的主要作用是允许一个或多个线程在某个特定条件成立之前进入等待状态，并在条件满足时被唤醒继续执行。
条件变量为解决复杂的线程同步问题提供了一种高效且灵活的方式。
*/
typedef struct share{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}shareRes, *pShareRes;

/**
 * 
 * pthread_wait的工作过程
 * 上半部
 * 1.去对应的条件变量排队
 * 2.解锁在加入等待队列后，pthread_cond_wait 会自动释放与条件变量关联的互斥锁（pthread_mutex_unlock），以便其他线程可以获取该锁并修改共享资源。
 * 3.进程被挂起，睡觉
 * 下半部
 * 被解锁时pthread_wait要
 * 1.唤醒
 * 2.加锁
 * 3.返回
 */

void *thread_fun(void * p){
    pShareRes ptr = (pShareRes)p;
    pthread_mutex_lock(&ptr->mutex);
    pthread_cond_wait(&ptr->cond, &ptr->mutex); // 与mutex结合使用
    pthread_mutex_unlock(&ptr->mutex);
    printf("I am child thread\n");
    return NULL;    // 并不会检查清理栈
}

int main(){
    pthread_t tid;
    shareRes share;
    pthread_cond_init(&share.cond, NULL); // 动态初始化条件变量
    pthread_mutex_init(&share.mutex, NULL); 
    int ret = pthread_create(&tid, NULL, thread_fun, &share);
    THREAD_ERR_CHECK(ret, "pthread_create");
    printf("I am main thread\n");
    sleep(1);
    pthread_cond_signal(&share.cond);   // 唤醒在cond中的线程, 这个是一瞬间的，所以要确保在这之前先设计了条件变量
    long pret;
    pthread_join(tid, (void **)&pret);
    printf("return %ld\n", pret);  // 正常退出返回的是0，异常退出返回-1
    return 0;
}