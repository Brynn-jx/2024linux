#include<func.h>

typedef struct share{
    int val;
    pthread_mutex_t mutex;
}shareRes, *pShareRes;

#define N 1000000;

void *thread_fun(void * p){
    pShareRes ptr = (pShareRes)p;
    for(int i = 0; i < 1000000; i++){
        pthread_mutex_lock(&ptr->mutex);
        ptr->val++;
        pthread_mutex_unlock(&ptr->mutex);
    }   
    return NULL;    // 并不会检查清理栈
}

int main(){
    pthread_t tid;
    shareRes share;
    pthread_mutex_init(&share.mutex, NULL); 
    int ret = pthread_create(&tid, NULL, thread_fun, &share);
    THREAD_ERR_CHECK(ret, "pthread_create");
    for (int i = 0; i < 1000000; i++){
        pthread_mutex_lock(&share.mutex);
        share.val++;
        pthread_mutex_unlock(&share.mutex);
    }
    long pret1;
    pthread_join(tid, (void **)&pret1);
    printf("return %ld\n", pret1);  // 正常退出返回的是0，异常退出返回-1
    printf("%d\n", share.val);
    return 0;
}