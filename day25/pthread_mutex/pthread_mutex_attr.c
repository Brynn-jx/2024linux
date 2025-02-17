#include<func.h>

typedef struct share{
    pthread_mutex_t mutex;
}shareRes, *pShareRes;

void *thread_fun(void * p){
    pShareRes ptr = (pShareRes)p;
    pthread_mutex_lock(&((pShareRes)p)->mutex); // 加锁
    printf("I am child thread\n");
    pthread_mutex_unlock(&((pShareRes)p)->mutex);   // 解锁
    return NULL;    // 并不会检查清理栈
}

/*
锁分为两类一类是 睡眠锁 
一类是 自旋锁 这类锁不会睡眠会不停的等待常用于性能要求高的地方，如内核
 */
int main(){
    pthread_t tid;
    pthread_mutexattr_t attr;
    shareRes share;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);  // 递归锁可以重复加锁
    pthread_mutex_init(&share.mutex, &attr); 
    int ret = pthread_create(&tid, NULL, thread_fun, &share);
    THREAD_ERR_CHECK(ret, "pthread_create");
    pthread_mutex_lock(&share.mutex);
    pthread_mutex_lock(&share.mutex);
    printf("I am main thread\n");
    pthread_mutex_unlock(&share.mutex);
    pthread_mutex_unlock(&share.mutex);
    long pret;
    pthread_join(tid, (void **)&pret);
    printf("return %ld\n", pret);  // 正常退出返回的是0，异常退出返回-1
    return 0;
}