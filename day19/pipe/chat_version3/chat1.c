#include<func.h>

int main(int argc, char *argv[]){
    ARGS_CHECK(argc, 3);
    int fdr = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open");
    int fdw = open(argv[2], O_WRONLY);
    ERROR_CHECK(fdw, -1, "open");
    char buf[1024] = {0};
    fd_set readset;
    int ret = 0;
    struct timeval t;
    t.tv_sec = 1; 
    t.tv_usec = 0;
    while (1){
        FD_ZERO(&readset);
        FD_SET(STDIN_FILENO, &readset);
        FD_SET(fdr, &readset);
        t.tv_sec = 1; 
        t.tv_usec = 0;
        int ready_num = select(fdr + 1, &readset, NULL, NULL, &t);
        if(ready_num > 0){
            if(FD_ISSET(STDIN_FILENO, &readset)){ // 标准输入准备好了 写入2管道
                memset(buf, 0, sizeof(buf));
                ret = read(STDIN_FILENO, buf, sizeof(buf));
                ERROR_CHECK(ret, -1, "read");
                if(0==ret){
                    printf("我想结束\n");//ctrl d
                    break;
                }
                write(fdw, buf, strlen(buf));
            }
            if(FD_ISSET(fdr, &readset)){
                memset(buf, 0, sizeof(buf));
                ret = read(fdr, buf, sizeof(buf));
                ERROR_CHECK(ret, -1, "read");
                if(ret == 0){
                    printf("对方断开\n");
                    break;
                }
                printf("%s", buf);
            }
        }else{
            time_t now =  time(NULL);
            printf("超时 %s\n", ctime(&now));
        }
        
    }
    

    return 0;
}