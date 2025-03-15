#include "head.h"

int transFile(int new_fd, char* file_name){  // 发送文件
    // 不用再发送文件名了，因为已经用send_protocol发送过了
    train_t train;
    // 再发送文件内容
    int fd = open(file_name, O_RDONLY);

    // 发送文件大小
    struct stat file_buf;
    fstat(fd, &file_buf);   // 获取文件状态
    train.size = sizeof(file_buf.st_size); // 获取存放文件大小的变量的字节数
    memcpy(train.buf, &file_buf.st_size, sizeof(file_buf.st_size)); // 将字节数复制到train.buf中
    send(new_fd, &train, 4 + train.size, 0);    // 发送文件所占空间大小和存放大小所占的字节数   
    
    // 发送文件内容
    long ret = sendfile(new_fd, fd, 0, file_buf.st_size);   // sendfile不是退出点
    ERROR_CHECK(ret, -1, "sendfile");
    
end:
    close(fd);
    return 0;
}

//循环接收，要接多少字节，接完那么多才会返回
int recvn(int new_fd,void* pStart,int len){
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
// int recvFile(int new_fd,char* file_name)
// {
//     int fd=open(file_name,O_RDWR|O_CREAT|O_APPEND,0666);
//     ERROR_CHECK(fd,-1,"open");
//     int recvLen;
//     //接文件大小
//     off_t fileSize;//总长度
//     recvn(new_fd,&recvLen,4);
//     recvn(new_fd,&fileSize,recvLen);

//     struct timeval start,end;
//     gettimeofday(&start,NULL);


//     off_t downLoadSize=0;
//     off_t lastSize=0;
//     off_t slice=fileSize/100;//每百分之一
//     char buf[1000];

//     //接文件内容,接1000的整数倍
//     while(fileSize-downLoadSize>=1000)
//     {
//         recvn(new_fd,buf,1000);//接recvLen长度的数据
//         write(fd,buf,1000);
//         downLoadSize+=1000;
//         if(downLoadSize-lastSize>slice)
//         {
//             printf("%5.2lf%%\r", 100.0*downLoadSize/fileSize);
//             fflush(stdout);
//             lastSize = downLoadSize;
//         }
//     }
//     recvn(new_fd,buf,fileSize-downLoadSize); //接最后小于1000个字节的部分
//     write(fd,buf,fileSize-downLoadSize);
    
//     printf("100.00%%\n");
//     gettimeofday(&end,NULL);
//     //统计下载总时间
//     printf("use time=%ld\n",(end.tv_sec-start.tv_sec)*1000000+end.tv_usec-start.tv_usec);
//     close(fd);//关闭文件
//     return 0;
// }

// //接收文件
int recvFile(int new_fd, char *file_name){
    int fd = open(file_name, O_CREAT | O_WRONLY, 0666);
    // splice 系统调用本身并不直接支持 O_APPEND 标志 所以手动将文件描述符放到末尾
    lseek(fd, 0, SEEK_END);
    ERROR_CHECK(fd,-1,"open");
    int recvLen;
    off_t file_size = 0;
    char buf[1000]={0};
    // 获取文件大小
    recvn(new_fd,&recvLen,4);
    recvn(new_fd, &file_size, recvLen);

    off_t lastSize=0;
    off_t slice=file_size/100;//每百分之一

    struct timeval start, end;
    gettimeofday(&start, NULL);

    int pipefd[2];
    pipe(pipefd); // 建立匿名管道 创建时规定pipefd[0]是读端，pipefd[1]是写端

    off_t total = 0;
    printf("file_szie = %ld\n", file_size);
    while(total < file_size){
        //从new_fd读取4096字节数据，写入管道,ret是代表写入管道的数据量
        ssize_t  ret =splice(new_fd,NULL,pipefd[1],NULL,4096,SPLICE_F_MORE);
        printf("%ld\n", ret);
        if(ret<=0)
        {
            // printf("对端断开了\n");
            break;
        }
        total += ret;
        if(total-lastSize>slice)
        {
            printf("%5.2lf%%\r", 100.0*total/file_size);
            fflush(stdout);
            lastSize = total;
        }
        //从管道中读取数据，写入到文件，写ret个字节
        ret=splice(pipefd[0],NULL,fd,NULL,ret,SPLICE_F_MORE);
        ERROR_CHECK(ret, -1, "splice");
        if(ret<=0)
        {
            printf("对端断开了\n");
            break;
        }
    }
    
    printf("100.00%%\n");
    gettimeofday(&end, NULL);
    // 计算所用时间
    printf("usec = %ld\n", (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec));
    close(fd);//关闭文件
    return 0;
}

//buf里只能是字符串,发数据
int send_protocol(int new_fd,int control_code,void *pdata, int send_size){
    protocol_t t;
    int ret;
    t.size=send_size;//火车头部
    t.control_code=control_code;
    if(pdata)
        memcpy(t.buf, pdata, send_size);
    ret=send(new_fd,&t,8+t.size,0);
    ERROR_CHECK(ret,-1,"send");
    return 0;
}

//buf里只能是字符串，接数据
int recv_protocol(int new_fd,int *control_code,void *pdata){
    int recvLen;
    recvn(new_fd,&recvLen, 4);//接火车车厢
    recvn(new_fd,control_code, 4);//接控制码
    recvn(new_fd,pdata,recvLen);
    return 0;
}