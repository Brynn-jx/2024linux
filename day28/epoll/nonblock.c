#include<func.h>

// 设置为非阻塞，通常与循环配合使用实现一种同步非阻塞的效果
int setNonBlock(int fd){
    int status, ret;
    status = fcntl(fd, F_GETFL);
    status |= O_NONBLOCK;
    ret = fcntl(fd, F_SETFL, status);
    return 0;
}

int main(){
    setNonBlock(STDIN_FILENO);
    char buf[30] = {0};
    read(STDIN_FILENO, buf, sizeof(buf));
    printf("buf = %s\n", buf);
    return 0;
}