#include <func.h>

//循环接收，要接多少字节，接完那么多才会返回
int recvn(int new_fd,void* pStart,int len)
{
    int total=0,ret;
    char *p=(char*)pStart;
    while(total<len)
    {
        ret=recv(new_fd,p+total,len-total,0);//ret是接收到的字节数
        ERROR_CHECK(ret,-1,"recv");
        if(0==ret)
        {
            return -1;//对端断开了，返回-1
        }
        total+=ret;
    }
    return 0;
}

//接收文件
int recvFile(int new_fd)
{
    //先接文件名,每次先接4个字节
    int recvLen;
    char buf[1000]={0};
    recvn(new_fd,&recvLen,4);
    recvn(new_fd,buf,recvLen);
    printf("buf=%s\n",buf);
    int fd=open(buf,O_RDWR|O_CREAT,0666);
    ERROR_CHECK(fd,-1,"open");

    //接文件大小
    off_t fileSize;
    recvn(new_fd,&recvLen,4);
    recvn(new_fd,&fileSize,recvLen);
    printf("recvLen = %d\n", recvLen);

    off_t downLoadSize=0;
    off_t lastSize=0;
    off_t slice=fileSize/100;//每百分之一

    printf("fileSize = %ld, slice = %ld\n", fileSize, slice);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    int pipefd[2];
    pipe(pipefd); // 建立匿名管道 创建时规定pipefd[0]是读端，pipefd[1]是写端

    int total;
    while (total < fileSize){   
        int ret = splice(new_fd, NULL, pipefd[1], NULL, 4096, SPLICE_F_MORE); // 管道空间有限制
        total += ret;
        splice(pipefd[0],NULL,fd,NULL,ret,SPLICE_F_MORE);
    }
    
    printf("100.00%%\n");
    gettimeofday(&end, NULL);
    // 计算所用时间
    printf("usec = %ld\n", (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec));
    close(fd);//关闭文件
    return 0;
}