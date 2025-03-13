#include"factory.h"

// 子线程函数

void cleanup(void *p) {
    pthread_mutex_t *pmutex=(pthread_mutex_t *)p;
    pthread_mutex_unlock(pmutex);//解锁
}


// 子线程的流程
void *threadFunc(void *p) {
    factory_t *pFactory = (factory_t *)p;
    taskQueue_t *pQue = &pFactory->taskQueue;
    task_t *pnew;
    int taskFlag;
    while(1){
        pthread_mutex_lock(&pQue->mutex); // 加锁
        pthread_cleanup_push(cleanup,&pQue->mutex);//清理函数
        if(!pQue->queueSize){ // 队列为空
            // 等待条件变量，释放互斥锁并进入等待状态 被唤醒的线程会重新获取互斥锁，并再次检查条件。
            pthread_cond_wait(&pFactory->cond, &pQue->mutex); 
        }
        if(pFactory->exitFlag){ // 处理被唤醒的线程
            printf("子线程退出\n");
            pthread_exit(NULL);
        }
        taskFlag = taskDeQueue(pQue, &pnew);
        pthread_cleanup_pop(1); // 非零，则立即执行该清理处理程序；如果为零，则仅弹出而不执行。用来取代解锁行为，同时当使用pthread_cancel时会执行之前push的清理函数
        if(!taskFlag){  // 拿到任务
            printf("send file\n");
            transFile(pnew->new_fd);
            close(pnew->new_fd);
            free(pnew);
        }
        if(pFactory->exitFlag){ // 处理做完任务的线程
            printf("子线程退出\n");
            pthread_exit(NULL);
        }
    }
}