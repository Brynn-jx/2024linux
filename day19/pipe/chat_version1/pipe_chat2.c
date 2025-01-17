#include<func.h>

//管道是半双工通信
// 写a管道
int main(){
    int fd = open("a.pipe", O_RDWR | O_CREAT, 0755);
    ERROR_CHECK(fd, -1, "open");
    char buf[1024] = {0};
    read(fd, buf, sizeof(buf)); // 读管道时，管道内没有数据会阻塞
    printf("%s", buf);
    close(fd);
    return 0;
}