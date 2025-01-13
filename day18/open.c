#include<func.h>

int main(int argc, char *argv[]){
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDWR | O_CREAT);
    char buf[1024] = {0};
    int len = read(fd, buf, sizeof(buf));
    printf("%d\n", fd);
    printf("%s\n", buf);
    printf("%d\n", len);
    close(fd);
    return 0;
}