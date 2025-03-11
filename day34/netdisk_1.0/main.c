#include"factory.h"

int pipe_exit[2];
int exit_flag = 0;
void exit_sigfunc(int signum){
    printf("要有序退出了\n");
    write(pipe_exit[1], "1", 1); // 1是写的端口
    exit_flag = 1;
}

int main(int argc,char* argv[]){
    ARGS_CHECK(argc, 4);
    pipe(pipe_exit); // 初始化管道
    // 子进程若是异常退出则重新创建子进程和子线程
    while(fork()){ // 父进程操作接收退出信号，执行exit_sigfunc函数，让子进程中的线程有序退出，并且等待子进程退出
        signal(SIGUSR1, exit_sigfunc);
        wait(NULL); // 子进程无论是异常终止还是正常终止都会到这，并且处理后不再阻塞
        if(exit_flag){
            exit(0);
        }
        printf("子进程异常退出，重新启动子进程\n");
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

    int ready_fd_num, epoll_ctl_num = 2;    // 现在监控的数目
    struct epoll_event evs[100]; // 现在默认的监控能够并发的数目是100

    while(1){
        ready_fd_num = epoll_wait(epfd, evs, epoll_ctl_num, -1);
        for(int i = 0; i < ready_fd_num; i++){
            if(evs[i].data.fd == sock_fd){
                addrLen = sizeof(clientAddr);
                // 客户端发来请求，一开始是注册或者登录
                new_fd = accept(sock_fd, (struct sockaddr*)&clientAddr, &addrLen);
                ERROR_CHECK(new_fd, -1, "accept");
                // 来了就先监听，验证通过再分配给子线程
                epoll_add(epfd,new_fd);
                epoll_ctl_num += 1;
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
            }else{
                int control_code;
                char user_plus_passwd[100]={0};
                user_passwd_t user_passwd;
                bzero(&user_passwd, sizeof(user_passwd_t));
                recv_protocol(evs[i].data.fd, &control_code, user_plus_passwd);
                printf("%s\n", user_plus_passwd);
                if(control_code == 1){  // 注册
                    split_user_passwd(user_plus_passwd, user_passwd.username, user_passwd.password);
                }else if(control_code == 2){
                    
                }
                printf("%s %s\n", user_passwd.username, user_passwd.password);
                while(1);
                printf("主线程放任务到队列\n");
                // 放入队列
                taskEnQueue(pQue, new_fd);
                pthread_cond_signal(&factory.cond); // 唤醒一个子线程
            }
        }
    }
    close(sock_fd);
    return 0;
}