#include<func.h>
// 打开顺序很重要，读写两端的进程要按正确顺序打开管道，否则会导致阻塞或死锁。在实际应用中，需要仔细设计进程间的同步机制，避免死锁和阻塞。
// 因为 open 操作会阻塞进程直到另一个进程打开管道的另一端
int main(){
    // 读取a管道 写b管道
    int fd1 = open("a", O_RDONLY);
    ERROR_CHECK(fd1, -1, "open");
    int fd2 = open("b", O_WRONLY);
    ERROR_CHECK(fd2, -1, "open");
    printf("fd1 = %d fd2 = %d 开始通信\n", fd1, fd2);
    char buf[1000] = {0};
    while (1){
        memset(buf, 0, sizeof(buf));
        // 读取a管道
        read(fd1, buf, sizeof(buf));
        printf("%s", buf);
        memset(buf, 0, sizeof(buf));
        read(STDIN_FILENO, buf, sizeof(buf));   // 先标准读输入 再写到b中
        write(fd2, buf, strlen(buf));
    }
    close(fd1);
    close(fd2);
    return 0;
}
