#include"factory.h"

// 在客户端文件名就是md5值
int transFile(int new_fd, char* file_name, off_t seek_pos, off_t remain_size){  // 发送文件
    char path_file_name[1000]="net_root/";
    strcat(path_file_name, file_name);
    // 不用再发送文件名了，因为已经用send_protocol发送过了
    train_t train;
    // 再发送文件内容
    int fd = open(path_file_name, O_RDONLY);
    ERROR_CHECK(fd, -1, "open");

    // 发送文件大小
    train.size = sizeof(remain_size); // 获取存放文件大小的变量的字节数
    memcpy(train.buf, &remain_size, train.size); // 将字节数复制到train.buf中
    send(new_fd, &train, 4 + train.size, 0);    // 发送文件所占空间大小和存放大小所占的字节数   
    
    // 发送文件内容
    long ret;
    if(seek_pos)
        ret = sendfile(new_fd, fd, &seek_pos, remain_size);   // sendfile不是退出点
    else
        ret = sendfile(new_fd, fd, NULL, remain_size);
    ERROR_CHECK(ret, -1, "sendfile");
    
end:
    close(fd);
    return 0;
}

int recvn(int new_fd, void* pStart, int len){
    int total = 0, ret;
    char *p = (char *)pStart;
    while (total < len){
        ret = recv(new_fd, p + total, len - total, 0);  // 有可能没有读取到指定的字节数，就会返回，所以需要循环读取直到读取到指定的字节数
        total += ret; 
        ERROR_CHECK(ret,-1,"recv");
        if(0==ret){
            printf("对方断开了\n");
            return -1;//对端断开了，返回-1
        }
    }
    return 0;
}


off_t recvFile(int new_fd, char* md5_file_name){
    // 先接收文件名
    int recvLen;
    // 将客户上传的文件保存到net_root目录下
    char path_file_name[1000]="net_root/";
    int ret;

    strcat(path_file_name,md5_file_name);
    int fd=open(path_file_name,O_RDWR|O_CREAT,0666);
    ERROR_CHECK(fd,-1,"open");

    // 接收文件大小
    off_t file_size = 0;
    recvn(new_fd, &recvLen, 4);
    recvn(new_fd, &file_size, recvLen);

    struct timeval start,end;
    gettimeofday(&start,NULL);

    off_t downLoadSize=0;
    off_t lastSize=0;
    off_t slice=file_size/100;//每百分之一

    int pipefds[2];
    pipe(pipefds);//初始化管道 默认 1端口写 0端口读
    int total = 0;
    //使用splice再接文件内容
    while(total < file_size){
        //从new_fd读取4096字节数据，写入管道,ret是代表写入管道的数据量
        int ret =splice(new_fd,NULL,pipefds[1],NULL,4096,SPLICE_F_MORE);
        if(ret<=0)
        {
            printf("对端断开了\n");
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
        ret=splice(pipefds[0],NULL,fd,NULL,ret,SPLICE_F_MORE);
        if(ret<=0)
        {
            printf("对端断开了\n");
            break;
        }
    }
    printf("100.00%%\n");
    gettimeofday(&end,NULL);
    //统计下载总时间
    printf("use time=%ld\n",(end.tv_sec-start.tv_sec)*1000000+end.tv_usec-start.tv_usec);
    close(fd);//关闭文件
    return file_size;
}

// 可以发送任何数据
int send_protocol(int new_fd,int control_code, void* pdata, int send_size){
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

// 可以接收任何数据
int recv_protocol(int new_fd,int *control_code,void *pdata){
    int recvLen;
    int ret;
    ret = recvn(new_fd,&recvLen,4);//接火车车厢
    if(ret == -1){
        return -1;
    }
    recvn(new_fd,control_code,4);//接控制码
    if(ret == -1){
        return -1;
    }
    recvn(new_fd,pdata,recvLen);
    if(ret == -1){
        return -1;
    }
    return 0;
}