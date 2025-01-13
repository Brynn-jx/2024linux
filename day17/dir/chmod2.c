#include <func.h>

int main(int argc,char *argv[])
{
    if(argc!=2)
    {
        printf("error args\n");
        return -1;
    }
    int ret=chmod(argv[1],0777);//用来改变某个文件或者目录的权限
    ERROR_CHECK(ret,-1,"chmod");
    return 0;
}