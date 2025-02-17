#include<func.h>

void clean_up(void * p){
    printf("clean up begin\n");
    free(p);
    printf("clean up success\n");
}

void *thread_fun(void * p){
    char *c = malloc(20);
    pthread_cleanup_push(clean_up, (void *)c);
    printf("I am child thread\n");
    pthread_exit(NULL); // 会检查清理栈并且执行
    pthread_cleanup_pop(1); // push 和 pop 必须成对出现，1就代表将清理栈的内容弹出并且执行，0是掷弹出不执行
}

int main(){
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, thread_fun, NULL);
    THREAD_ERR_CHECK(ret, "pthread_create");
    printf("I am main thread\n");
    long pret;
    pthread_join(tid, (void **)&pret);
    printf("return %ld\n", pret);  // 正常退出返回的是0，异常退出返回-1
    return 0;
}