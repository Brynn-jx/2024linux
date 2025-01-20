#include<func.h>

int main(){
    int ret = system("python3 hello.py");
    ERROR_CHECK(ret, -1, "system");
    return 0;
}