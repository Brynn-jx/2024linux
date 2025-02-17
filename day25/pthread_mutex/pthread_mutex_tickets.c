#include<func.h>

typedef struct share{
    int val;
    pthread_mutex_t mutex;
}shareRes, *pShareRes;

#define N 10000000;
void *thread_fun_ticket1(void * p){
    pShareRes ptr = (pShareRes)p;
    while(1){
        pthread_mutex_lock(&ptr->mutex); 
        if(ptr->val > 0){
            ptr->val--;
            printf("thread_fun_ticket1 sell ticket, remain tickets are %d\n", ptr->val);
            pthread_mutex_unlock(&ptr->mutex);
        }else{
            pthread_mutex_unlock(&ptr->mutex);
            break;
        }
    }
    return NULL;    // 并不会检查清理栈
}

void *thread_fun_ticket2(void * p){
    pShareRes ptr = (pShareRes)p;
    while(1){
        pthread_mutex_lock(&ptr->mutex); 
        if(ptr->val > 0){
            ptr->val--;
            printf("thread_fun_ticket2 sell ticket, remain tickets are %d\n", ptr->val);
            pthread_mutex_unlock(&ptr->mutex);
        }else{
            pthread_mutex_unlock(&ptr->mutex);
            break;
        }
    }
    return NULL;    // 并不会检查清理栈
}

int main(){
    pthread_t tid1, tid2;
    shareRes share;
    share.val = 10000;
    pthread_mutex_init(&share.mutex, NULL); 
    int ret = pthread_create(&tid1, NULL, thread_fun_ticket1, &share);
    THREAD_ERR_CHECK(ret, "pthread_create");
    ret = pthread_create(&tid2, NULL, thread_fun_ticket2, &share);
    THREAD_ERR_CHECK(ret, "pthread_create");
    printf("I am main thread\n");
    long pret1, pret2;
    pthread_join(tid1, (void **)&pret1);
    pthread_join(tid2, (void **)&pret2);
    printf("return %ld %ld\n", pret1, pret2);  // 正常退出返回的是0，异常退出返回-1
    return 0;
}