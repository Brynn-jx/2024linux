#include<func.h>

void *thread_fun(void* arg){
    printf("I am child thread\n");
    char *ret = malloc(20);
    strcpy(ret, "hello");
    printf("%p %ld\n", (void *)ret, sizeof(ret));
    return (void*) ret;
}

int main(){
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, thread_fun, NULL);
    THREAD_ERR_CHECK(ret, "pthread_create");
    printf("I am main thread\n");
    char *pret;
    pthread_join(tid, (void **)&pret);  // 接受返回的值，其实就是将返回值void*的地址接受下来，再用对应的形式显示出来
    printf("child thread ret = %s\n", pret);
    printf("%p %ld\n", (void *)pret, sizeof(pret));
    return 0;
}