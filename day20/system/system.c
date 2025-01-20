#include<func.h>

int main(){
    int ret = system("ls -lh");
    ERROR_CHECK(ret, -1, "system");
    return 0;
}