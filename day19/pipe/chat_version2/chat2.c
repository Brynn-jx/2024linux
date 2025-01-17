#include<func.h>

int main(){
    // 读取b管道 写a管道
    int fd2 = open("a", O_WRONLY);
    ERROR_CHECK(fd2, -1, "open");
    int fd1 = open("b", O_RDONLY);
    ERROR_CHECK(fd1, -1, "open");
    printf("fd1 = %d fd2 = %d 开始通信\n", fd1, fd2);
    char buf[1000] = {0};
    while (1){
        memset(buf, 0, sizeof(buf));
        read(STDIN_FILENO, buf, sizeof(buf));   // 先标准读输入
        write(fd2, buf, strlen(buf));   /// 发送给a管道
        memset(buf, 0, sizeof(buf));
        read(fd1, buf, sizeof(buf));    // 读取b
        printf("%s", buf);
    }
    close(fd1);
    close(fd2);
    return 0;
}
