#include<func.h>

// 实现ls -l的功能 main函数传入的参数是文件路径

char *chmod_file(mode_t mode){
    char *ret = malloc(16 * sizeof(char));
    ret[0] = '8';
    for(int i = 0; i < 9; i++){
        int j = mode % 2 ;
        mode  = mode >> 1;
        if(j == 1){
            if((i + 1) % 3 == 1)
                ret[9 - i] = 'x';
                
            if((i + 1) % 3 == 2)
                ret[9 - i] = 'w';
                
            if((i + 1) % 3 == 0)
                ret[9 - i] = 'r';
        }else{
            ret[9  - i] = '-';
        }
       
    }
    mode = mode >> 3;
    int temp = 0xf & mode;
    ret[0] = temp + '0';
    return ret;
}

int printfile(char *path){
    // 遍历整个目录
    DIR *dir = opendir(path);
    ERROR_CHECK(dir, NULL, "opendir");
    struct dirent* d = NULL;
    struct stat p;
    char path_buf[1024] = {0};
    while (d = readdir(dir)){
        // 输出ls -l的形式
         if(!strcmp(d->d_name, ".") || !strcmp(d->d_name, "..")){
            continue;
        }
        sprintf(path_buf, "%s/%s", path, d->d_name);
        int ret = stat(path_buf, &p);
        ERROR_CHECK(ret, -1, "stat");
        char *mode = malloc(16 * sizeof(char));
        mode = chmod_file(p.st_mode);
        printf("inode=%lu mode = %s link=%ld uid=%s gid=%s size=%ld time=%s %s\n",
        p.st_ino, mode, p.st_nlink, getpwuid(p.st_uid)->pw_name,getgrgid(p.st_gid)->gr_name, p.st_size, ctime(&p.st_mtime) + 4, d->d_name);  
       
        // ls 只遍历当前文件夹的文件 不遍历所有文件
        // if(d->d_type == 4){
        //     char temp_path[1024] = {0};
        //     sprintf(temp_path, "%s/%s", path, d->d_name);
        //     printfile(temp_path);
        // }
    }
    closedir(dir);
}

int main(int argc, char *argv[]){
    ARGS_CHECK(argc, 2);
    struct stat buf; // 用来存储inode信息
    int ret = stat(argv[1], &buf);
    ERROR_CHECK(ret, -1, "stat");
    // printf("inode=%lu mode=%x link=%ld uid=%d gid=%d size=%ld time=%ld\n",
    // buf.st_ino, buf.st_mode, buf.st_nlink, buf.st_uid, buf.st_gid, buf.st_size, buf.st_mtime); 
    printfile(argv[1]);
    return 0;
    // 将mode转换为rwxrwxrwx的形式 高4位是文件类型 低9位是文件权限
    // ctime将秒数时间转化为字符串时间 gtime返回一个时间结构体
    // printf + 4将从第四个字符的位置进行输出，想象成字符数组
}