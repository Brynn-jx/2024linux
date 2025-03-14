#include"factory.h"

// 初始化
int factoryInit(factory_t *pFactory, int threadNum){
    int ret;
    bzero(pFactory, sizeof(factory_t));
    pFactory->thidAddr = (pthread_t *)calloc(threadNum, sizeof(pthread_t));
    pFactory->threadNum = threadNum;
    ret = pthread_cond_init(&pFactory->cond, NULL); // 条件变量初始化
    THREAD_ERR_CHECK(ret, "pthread_cond_init");
    pthread_mutex_init(&pFactory->taskQueue.mutex, NULL); // 锁初始化
    return 0;
}

// 启动子线程
int factoryStart(factory_t *pFactory){
    int i, ret;
    for(i = 0; i < pFactory->threadNum; i++){
        // 调用threadFunc函数来进行子进程的处理
        ret = pthread_create(pFactory->thidAddr + i, NULL, threadFunc, pFactory);
        printf("create thread %ld\n", pFactory->thidAddr[i]);
    }
}

//注册监控某个描述符是否可读
int epoll_add(int epfd,int fd){
    struct epoll_event event;
    event.data.fd=fd;
    event.events=EPOLLIN;
    int ret=epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl");
    return 0;
}