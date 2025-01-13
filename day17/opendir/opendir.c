#include<func.h>

int main(int argc, char *argv[]){
    ARGS_CHECK(argc, 2);
    DIR *dir = opendir(argv[1]); 
    struct dirent *p = NULL;
    off_t offset;   // 记录在磁盘上的偏移距离
    while (p = readdir(dir)){
        printf("inode=%lu,filename_len=%u,type=%d,filename=%s\n",p->d_ino,p->d_reclen,
    p->d_type,p->d_name);
        if(strcmp(p->d_name, "Makefile") == 0){
            offset = telldir(dir);
        }
    }
    seekdir(dir, offset);
    p=readdir(dir);
    printf("----------------------------------------------\n");
    printf("inode=%lu,filename_len=%u,type=%d,filename=%s\n",p->d_ino,p->d_reclen,
    p->d_type,p->d_name);

    closedir(dir);
    return 0;
}