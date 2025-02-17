#include<func.h>

typedef struct share{
    pthread_mutex_t mutex;
}shareRes, *pShareRes;

void *thread_fun(void * p){
    pShareRes ptr = (pShareRes)p;
    pthread_mutex_lock(&((pShareRes)p)->mutex); // 加锁
    printf("I am child thread\n");
    return NULL;    // 并不会检查清理栈
}

int main(){
    pthread_t tid;
    shareRes share;
    pthread_mutex_init(&share.mutex, NULL); 
    int ret = pthread_create(&tid, NULL, thread_fun, &share);
    THREAD_ERR_CHECK(ret, "pthread_create");
    sleep(1);
    pthread_mutex_trylock(&share.mutex);    // 若是已加锁不会阻塞继续执行，若是加锁了就是正常的加锁
    printf("I am main thread\n");
    pthread_mutex_unlock(&share.mutex);
    long pret;
    pthread_join(tid, (void **)&pret);
    printf("return %ld\n", pret);  // 正常退出返回的是0，异常退出返回-1
    return 0;
}
