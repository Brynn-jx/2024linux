#include"factory.h"

int pipe_exit[2];
void exit_sigfunc(int signum){
    printf("要有序退出了\n");
    write(pipe_exit[1], "1", 1); // 1是写的端口
}

int main(int argc,char* argv[]){
    ARGS_CHECK(argc, 4);
    pipe(pipe_exit); // 初始化管道
    // 有序退出
    if(fork()){ // 父进程操作接收信号，并且等待子进程退出
        signal(SIGUSR1, exit_sigfunc);
        wait(NULL);
        exit(0);
    }

    int threadNum = atoi(argv[3]); // 传入的进程数目
    factory_t factory;  // 主数据结构，线程池工厂
    factoryInit(&factory, threadNum); // 工厂初始化
    factoryStart(&factory); // 启动线程

    int sock_fd, new_fd;

    tcp_init(argv[1], argv[2], &sock_fd);   //网络初始化

    struct sockaddr_in clientAddr;
    socklen_t addrLen;

    taskQueue_t *pQue = &factory.taskQueue;
    // 注册监控sock_fd
    int epfd = epoll_create(1);
    epoll_add(epfd, sock_fd);

    // 注册监控exit_pipe[0]
    epoll_add(epfd, pipe_exit[0]);

    int ready_fd_num;
    struct epoll_event evs[2];

    while(1){
        ready_fd_num = epoll_wait(epfd, evs, 2, -1);
        for(int i = 0; i < ready_fd_num; i++){
            if(evs[i].data.fd == sock_fd){
                addrLen = sizeof(clientAddr);
                new_fd = accept(sock_fd, (struct sockaddr*)&clientAddr, &addrLen);
                ERROR_CHECK(new_fd, -1, "accept");
                printf("主线程放任务到队列\n");
                // 放入队列
                taskEnQueue(pQue, new_fd);
                pthread_cond_signal(&factory.cond); // 唤醒一个子线程
            }
            else if(evs[i].data.fd == pipe_exit[0]){    // 处理退出信号
                printf("线程池开始退出\n");
                pthread_mutex_lock(&pQue->mutex);
                factory.exitFlag = 1;   // 1为退出标记
                pthread_mutex_unlock(&pQue->mutex);
                for(int j = 0; j < threadNum; j++){
                    // 都唤醒
                    pthread_cond_signal(&factory.cond);
                }
                for(int j = 0; j < threadNum; j++){
                    pthread_join(factory.thidAddr[j], NULL);
                }
                printf("线程池退出成功\n");
                exit(0);
            }
        }
    }
    close(sock_fd);
    return 0;
}