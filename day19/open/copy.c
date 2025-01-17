#include<func.h>

// 实现从文件的拷贝
int main(int argc, char* argv[]){
    ARGS_CHECK(argc, 3);
    int fd1 = open(argv[1], O_RDWR);
    ERROR_CHECK(fd1, -1, "open");
    int fd2 = open(argv[2], O_CREAT | O_RDWR, 0755);
    char buf[1024] = {0};
    while(read(fd1, buf, sizeof(buf))){
        write(fd2, buf, strlen(buf));
    }
    close(fd1);
    close(fd2);
    return 0;
}