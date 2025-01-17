#include<func.h>

//管道是半双工通信
// 写a管道
int main(){
    int fd = open("a.pipe", O_RDWR | O_CREAT, 0755);
    ERROR_CHECK(fd, -1, "open");
    write(fd, "hello world", 11);
    close(fd);
    return 0;
}