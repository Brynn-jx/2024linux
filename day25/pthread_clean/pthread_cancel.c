#include<func.h>

void clean_up(void * p){
    printf("clean up begin\n");
    free(p);
    printf("clean up success\n");
}

void *thread_fun(void * p){
    char *c = malloc(100000);
    pthread_cleanup_push(clean_up, (void *)c);  // 一定要在清理点前push  
    sleep(1);
    printf("I am child thread\n");
    return NULL;    // 并不会检查清理栈
    pthread_cleanup_pop(1); // push 和 pop 必须成对出现，1就代表将清理栈的内容弹出并且执行，0是掷弹出不执行
}

int main(){
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, thread_fun, NULL);
    THREAD_ERR_CHECK(ret, "pthread_create");
    pthread_cancel(tid); // 会对线程清理栈进行检测并执行
    printf("I am main thread\n");
    long pret;
    pthread_join(tid, (void **)&pret);
    printf("return %ld\n", pret);  // 正常退出返回的是0，异常退出返回-1
    return 0;
}