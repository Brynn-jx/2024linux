#include<func.h>

int main(int argc, char* argv[]){
    ARGS_CHECK(argc, 2);
    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    char *p  = (char *)mmap(NULL, 10, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ERROR_CHECK((void *)p, -1, "mmap");
    printf("%s\n",p);
    p[0] = 'w';
    p[1] = 'j';
    p[2] = 'x';
    char *p1  = (char *)mmap(NULL, 10, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ERROR_CHECK((void *)p1, -1, "mmap");
    printf("%s\n",p1);
    munmap(p1, 10);
    close(fd);
    return 0;
}