#include<func.h>

int main(){
    // 将print的标准输出重定向为管道的写段 使用dup2
    FILE *fp = popen("./print", "r");
    char p[1000];
    fread(p, 1, 1000, fp);
    printf("%s", p);
    pclose(fp);
    return 0;
}