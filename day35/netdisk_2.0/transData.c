#include"factory.h"

int transFile(int new_fd){  // 发送文件
    // 先发送文件名
    train_t train;
    train.size = strlen("file1");
    strcpy(train.buf, "file1");
    send(new_fd, &train, train.size + 4, 0); // 发送的字节数为文件名大小和size，int类型4字节 size是存放的发送的字节数大小
    // 再发送文件内容
    int fd = open("file1", O_RDONLY);

    // 发送文件大小
    struct stat file_buf;
    fstat(fd, &file_buf);
    train.size = sizeof(file_buf.st_size);
    memcpy(train.buf, &file_buf.st_size, sizeof(file_buf.st_size));
    send(new_fd, &train, 4 + train.size, 0); 
    
    // 发送文件内容
    long ret = sendfile(new_fd, fd, 0, file_buf.st_size);   // sendfile不是退出点
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


int recvFile(int new_fd){
    // 先接收文件名
    train_t train;
    memset(train.buf, 0, sizeof(train.buf));
    train.size = 0;
    recvn(new_fd, &train.size, 4);
    recvn(new_fd, train.buf, train.size);
    printf("buf=%s\n",train.buf);
    int fd=open(train.buf,O_WRONLY|O_CREAT,0666);
    ERROR_CHECK(fd,-1,"open");


    //再接文件内容
    while(1){
        recvn(new_fd,&train.size, 4);//接火车车厢
        if(train.size>0)
        {
            recvn(new_fd,train.buf,train.size);//接recvLen长度的数据
            write(fd,train.buf,train.size);//把接收到的内容写入文件
        }else{
            break;
        }
    }
    close(fd);//关闭文件
    return 0;
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
    recvn(new_fd,&recvLen,4);//接火车车厢
    recvn(new_fd,control_code,4);//接控制码
    recvn(new_fd,pdata,recvLen);
    return 0;
}