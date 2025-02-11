#include<func.h>

int main(){
    FILE *fp = popen("./add", "w");
    ERROR_CHECK(fp, NULL, "popen");
    fwrite("3 4", 1, 3, fp);
    pclose(fp);
    return 0;
}