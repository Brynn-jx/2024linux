#include<func.h>

int main(int argc, char *argv[]){
    ARGS_CHECK(argc, 2);
    int fd1 = open(argv[1], O_RDWR | O_CREAT, 0755);
    int fd2 = dup2(fd1, STDOUT_FILENO);
    printf("%d\n", fd2);
    printf("493874410\n");
    printf("王佳♥\n");
    return 0;
}