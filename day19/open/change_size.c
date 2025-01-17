#include<func.h>

int main(int argc, char *argv[]){
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDWR | O_CREAT, 0755);
    ERROR_CHECK(fd, -1, "open");
    ftruncate(fd, 1024);
    close(fd);
    return 0;
}