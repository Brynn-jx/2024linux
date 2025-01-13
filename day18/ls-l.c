#include <func.h>

int main(int argc,char *argv[])
{
    ARGS_CHECK(argc,2);//必须传个参数
    int ret;
    DIR* dir;//目录指针
    dir=opendir(argv[1]);
    ERROR_CHECK(dir,NULL,"opendir");
    struct dirent *p;
    struct stat buf;//为了存储某个文件的inode信息
    char path_buf[1024]={0};
    while(p=readdir(dir))//读取目录
    {
        if(!strcmp(p->d_name,".")||!strcmp(p->d_name,".."))
        {
            continue;
        }
        //要进行路径拼接
        sprintf(path_buf,"%s/%s",argv[1],p->d_name);
        printf("%s\n",path_buf);
        ret=stat(path_buf,&buf);//获取文件状态(inode)信息
        ERROR_CHECK(ret,-1,"stat");
        printf("mode=%x,nlink=%ld,user=%s,%s,size=%ld,time=%s,%s\n",
        buf.st_mode,buf.st_nlink,getpwuid(buf.st_uid)->pw_name,
        getgrgid(buf.st_gid)->gr_name,buf.st_size,ctime(&buf.st_mtime)+4,p->d_name);
    }
    closedir(dir);


    return 0;
}