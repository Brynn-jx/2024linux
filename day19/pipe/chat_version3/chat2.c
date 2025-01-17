#include<func.h>

int main(int argc, char *argv[]){
    ARGS_CHECK(argc, 3);
    int fdw = open(argv[1] ,O_WRONLY);
    ERROR_CHECK(fdw, -1, "open");
    int fdr = open(argv[2], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open");
    char buf[1024] = {0};
    fd_set readset;
    int ret = 0;
    while (1){
        FD_ZERO(&readset);
        FD_SET(STDIN_FILENO, &readset);
        FD_SET(fdr, &readset);
        int read_num = select(fdr + 1, &readset, NULL, NULL, NULL);
        if(FD_ISSET(STDIN_FILENO, &readset)){
            memset(buf, 0, sizeof(buf));
            ret = read(STDIN_FILENO, buf, sizeof(buf));
            ERROR_CHECK(ret, -1, "read");
            if(ret == 0){
                printf("我要主动结束\n");
                break;
            }
            write(fdw, buf, strlen(buf));
        }
        if(FD_ISSET(fdr, &readset)){
            memset(buf, 0, sizeof(buf));
            ret = read(fdr, buf, sizeof(buf));
            ERROR_CHECK(ret, -1, "read");
            if(ret == 0){
                printf("对面主动结束\n");
                break;
            }
            printf("%s", buf);
        }

    }
    
    return 0;
}