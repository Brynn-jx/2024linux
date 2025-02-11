#include<func.h>

int main(int argc, char *argv[]){
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDONLY);
    char buf[1024] = {0};
    read(fd, buf, sizeof(buf));
    printf("%s\n", buf);
    close(fd);
    return 0;
}