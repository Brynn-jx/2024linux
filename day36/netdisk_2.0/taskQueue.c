#include "taskQueue.h"

int taskEnQueue(taskQueue_t *pQueue, int new_fd, int user_id){
    task_t *pnew = (task_t *)calloc(1, sizeof(task_t));
    pnew->new_fd = new_fd;
    pnew->user_id = user_id;
    pnew->path_id = 0;
    strcpy(pnew->path, "/");
    pthread_mutex_lock(&pQueue->mutex); // 上锁 
    // 尾插法
    if(!pQueue->rear){  // 队列为空
        pQueue->front = pnew;
        pQueue->rear = pnew;
    }
    else{
        pQueue->rear->pnext  = pnew;
        pQueue->rear = pnew;
    }
    pQueue->queueSize++;
    pthread_mutex_unlock(&pQueue->mutex); // 解锁
    return 0;
}

int taskDeQueue(taskQueue_t *pQueue, task_t **pnew){
    if(pQueue->queueSize){ // 需要判断队列中是否还有没分配的任务，因为一个任务可能被多个线程抢
    /**
     * 假设现在有A，B两个线程，A线程正在执行任务，B线程没有任务正在等待
     * 主线程现在发有一个任务，所以要唤醒一个线程，此时正好A线程完成了任务，率先拿到了锁
     * B线程也被唤醒了，但是唤醒时需要检测条件是否满足并且要重新获得锁，但是锁被A拿走了
     * 所以此时B线程会因为锁再次等待，直到A线程拿走了任务，释放了锁，此时B线程也会尝试获取
     * 任务，也就是将任务出队，所以需要这里写需要判断是否还有没分配的任务
     */
        *pnew = pQueue->front;
        pQueue->front = pQueue->front->pnext;
        pQueue->queueSize--;
        if(pQueue->front == NULL){
            pQueue->rear = NULL;
        }
    }
    else{
        return -1;
    }
    return 0;
}
