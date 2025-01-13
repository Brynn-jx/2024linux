#include<func.h>

int main(int argc, char *argv[]){
    ARGS_CHECK(argc, 2);
    char *path;
    path = getcwd(NULL, 0);
    printf("%s\n", path);
    int ret = chdir(argv[1]);
    ERROR_CHECK(ret, -1, "chdir");
    path = getcwd(NULL, 0);
    printf("%s\n", path);
    return 0;
}