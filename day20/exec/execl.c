//调用exec的程序代码
#include <func.h>
int main(){
    pid_t pid = fork();
    if(pid == 0){
        printf("I am child\n");
        //execl("./add","add","3","4",(char *)0);
        char *const argv[] = {"add","3","4",NULL};
        execv("./add",argv);
        printf("you can not see me!\n");//这句话并不会打印
    }
    else{
        printf("I am parent\n");
        printf("you can see me!\n");
        sleep(1);
    }
    return 0;
}