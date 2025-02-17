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

int main(){
    pthread_t tid;
    shareRes share;
    pthread_mutex_init(&share.mutex, NULL); 
    int ret = pthread_create(&tid, NULL, thread_fun, &share);
    THREAD_ERR_CHECK(ret, "pthread_create");
    printf("I am main thread\n");
    long pret;
    pthread_join(tid, (void **)&pret);
    printf("return %ld\n", pret);  // 正常退出返回的是0，异常退出返回-1
    return 0;
}