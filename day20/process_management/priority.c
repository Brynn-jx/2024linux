#include<func.h>

int main(){
    int fd = open("file", O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    printf("euid = %d egid = %d\n",  geteuid(), getegid());
    write(fd, "hello", 5);
    close(fd);
    return 0;
}