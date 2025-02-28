#include"head.h"

int makeChild(processData_t* workerList,int childNum)
{
    int i;
    pid_t pid;
    int pipefds[2];//管道
    for(i=0;i<childNum;i++)
    {
        socketpair(AF_LOCAL,SOCK_STREAM,0,pipefds);//初始化tcp类型管道
        pid=fork();
        if(0==pid)
        {
            close(pipefds[1]);//子进程关闭其中一端
            handleEvent(pipefds[0]);    // 子进程在没有被父进程处理之前就退出会变成僵尸进程导致循环异常
        }
        else{
            //父进程才会走到这里,保存子进程pid，初始化子进程状态
            close(pipefds[0]);//父进程关闭其中一端
            workerList[i].pid=pid;
            workerList[i].status=FREE;
            workerList[i].pipeFd=pipefds[1];
            printf("workerList[%d].pipeFd=%d\n",i,workerList[i].pipeFd);
        }
    }
    return 0;
}

void handleEvent(int pipeFd){
    while(1);
}