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

// 启动子线程 并且将main函数中的factory传过来
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

//解除注册监控某个描述符
int epoll_del(int epfd,int fd){
    struct epoll_event event;
    event.data.fd=fd;
    event.events=EPOLLIN;
    int ret=epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl");
    return 0;
}

// 修改路径，将后一个斜杠和倒数第二个斜杠之间的内容删除
void modify_path_slash(char *path){
    int len = strlen(path);

    // 寻找倒数第二个斜杠的位置
    int second_last_slash ;
    for (int i = len-2; i >=0; i--) {
        if(path[i]=='/')
        {
            second_last_slash=i;
            break;
        }
    }
    path[second_last_slash+1]=0;
}
