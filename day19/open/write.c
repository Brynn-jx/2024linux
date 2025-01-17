#include<func.h>


int main(int argc, char *argv[]){
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDWR | O_CREAT, 0755);
    char buf[1024] = {0};
    scanf("%s", buf);
    int  ret = write(fd, buf, strlen(buf));
    ERROR_CHECK(ret, -1, "write");
    return 0;
} 