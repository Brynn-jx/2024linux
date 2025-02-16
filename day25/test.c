#include<func.h>

int main(){
    char *p = malloc(20);
    strcpy(p, "hello");
    printf("%ld\n", strlen(p));
    return 0;
}