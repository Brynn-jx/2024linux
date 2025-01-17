#include<func.h>

int main(int argc, char *argv[]){
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDWR | O_CREAT, 0755);
    int fd2 = dup(fd);
    char buf[1024] = {0};
    printf("%d\n", fd);
    printf("%d\n", fd2);
    write(fd, "hello world", 11);
    close(fd);
    // 如果不从重新设置文件指针的位置就会接着从写完文件的位置进行读取
    lseek(fd2, 0, SEEK_SET);
    read(fd2, buf, sizeof(buf));
    printf("%s\n", buf);
    return 0;
}