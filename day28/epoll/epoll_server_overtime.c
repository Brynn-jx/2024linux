#include<func.h>

int main(int argc, char *argv[]){
    ARGS_CHECK(argc, 3);
    int sockId = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    ERROR_CHECK(sockId, -1, "socket");
    struct sockaddr_in in_addr;
    memset(&in_addr, 0, sizeof(in_addr));
    in_addr.sin_family = AF_INET;
    in_addr.sin_addr.s_addr = inet_addr(argv[1]);
    in_addr.sin_port = htons(atoi(argv[2]));
    int reuse = 1;
    // 允许地址重用
    int ret = setsockopt(sockId, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    ERROR_CHECK(ret, -1, "setsockopt");

    ret = bind(sockId, (struct sockaddr *)&in_addr, sizeof(in_addr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(sockId, 5);
    ERROR_CHECK(ret, -1, "bind");

    int  newFd = 0;
    
    int  epfd = epoll_create(1); // 他的参数目前没有意义，只要大于1即可
    
    // 设置监听事件并放入内核监听事件集合，这个集合由红黑树维护
    struct epoll_event event, evs[2];
    event.data.fd = STDIN_FILENO;
    event.events = EPOLLIN;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    event.data.fd = sockId;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sockId, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    int readyNum = 0;
    char buf[30];
    time_t new_time = time(NULL), last_time = time(NULL);
    int login = 0;

    while(1){
        readyNum = epoll_wait(epfd, evs, 2, 1000);
        printf("%d\n", readyNum);
        if(readyNum == 0 && login == 1){
            new_time = time(NULL);
            if(new_time - last_time > 3){
                login = 0;
                printf("overtime!\n");
                close(newFd);
            }
        }
        for(int i = 0; i < readyNum; i++){
            last_time = time(NULL);
            if(evs[i].data.fd == STDIN_FILENO){
                memset(buf, 0, sizeof(buf));
                read(STDIN_FILENO, buf, sizeof(buf));
                send(newFd, buf, strlen(buf) - 1,  0);
            }
           else if(evs[i].data.fd == newFd){
                memset(buf, 0, sizeof(buf));
                ret = recv(newFd, buf, sizeof(buf), 0);
                if(0 == ret){
                    printf("byebye\n");
                        close(sockId);
                        close(newFd);
                        return 0;
                    }
                    printf("buf=%s\n",buf);
            }
            else if(evs[i].data.fd == sockId){
                login = 1;
                newFd = accept (sockId, NULL, NULL);
                event.data.fd = newFd;
                ret = epoll_ctl(epfd, EPOLL_CTL_ADD, newFd, &event);
                ERROR_CHECK(ret, -1, "epoll_ctl");
            }
            
        }
    }
    close(sockId);
    close(newFd);
    return 0;
}