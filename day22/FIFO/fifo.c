#include<func.h>

int main(int argc, char *argv[]){
    ARGS_CHECK(argc, 2);
    int ret = mkfifo(argv[1], 0600);
    ERROR_CHECK(ret, -1, "mkfifo");
    int fd = open(argv[1], O_WRONLY);
    write(fd,"hello",5);    // 在管道的一端如果不读取的话就会被一直阻塞
    unlink(argv[1]);//使用unlink可以删除管道文件
    close(fd);
    return 0;
}