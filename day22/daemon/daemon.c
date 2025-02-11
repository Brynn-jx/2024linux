#include<func.h>

void Daemon(){
    const int MAXFD = 64;
    // 称为孤儿进程
    if(fork() != 0){
        exit(0);
    }
    // 成立新会话
    setsid();
    // 改变路径为根目录
    chdir("/");
    // 重设文件掩码
    umask(0);
    //关闭不需要的文件描述符
    for(int i = 0; i < MAXFD; i++){
        close(i);
    }
}


int main(){
    Daemon();
    // 下面就是守护进程的逻辑
    printf("pid = %d, ppid = %d pgid = %d\n", getpid(), getppid(), getpgid(0));
    while(1);
    return 0;
}