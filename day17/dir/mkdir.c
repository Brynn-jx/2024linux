#include<func.h>

int main(int argc, char* argv[]){
    ARGS_CHECK(argc, 3);
    char* c = NULL;
    unsigned long mode = strtol(argv[2], &c, 10);
    int ret =  mkdir(argv[1], mode);
    ERROR_CHECK(ret, -1, "mkdir");
    printf("%s\n", c);
    return 0;
}