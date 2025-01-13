#include<func.h>

// 深度优先遍历所有的文件夹
int print_dir(char *path){
    DIR *dir = opendir(path);
    ERROR_CHECK(dir,NULL,"opendir");
    struct dirent *p = NULL;
    while (p = readdir(dir)){
        printf("inode=%lu,filename_len=%u,type=%d,filename=%s\n", p->d_ino,
        p->d_reclen, p->d_type, p->d_name);
        if(strcmp(p->d_name, ".")  == 0 || strcmp(p->d_name, "..") == 0){
            continue;
        }
        // type = 4是文件夹，type=8是文件
        if(p->d_type == 4){
            // 拼接路径
            char temp_path[1024] = {0};
            sprintf(temp_path, "%s/%s", path, p->d_name);
            print_dir(temp_path);
        }
    }
    closedir(dir);
    return 0;   
}


int main(int argc, char *argv[]){
    ARGS_CHECK(argc, 2);
    print_dir(argv[1]);
    return 0;
}