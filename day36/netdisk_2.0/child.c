#include"factory.h"

// 子线程函数
void cleanup(void *p) {
    pthread_mutex_t *pmutex=(pthread_mutex_t *)p;
    pthread_mutex_unlock(pmutex);//解锁
}


// 子线程的流程
void *threadFunc(void *p) {
    factory_t *pFactory = (factory_t *)p;
    taskQueue_t *pQue = &pFactory->taskQueue;
    task_t *pnew;
    int taskFlag;
    MYSQL *conn;
    while(1){
        pthread_mutex_lock(&pQue->mutex); // 加锁
        pthread_cleanup_push(cleanup,&pQue->mutex);//清理函数
        if(!pQue->queueSize){ // 队列为空
            // 等待条件变量，释放互斥锁并进入等待状态 被唤醒的线程会重新获取互斥锁，并再次检查条件。
            pthread_cond_wait(&pFactory->cond, &pQue->mutex); 
        }
        if(pFactory->exitFlag){ // 处理被唤醒的线程
            printf("子线程退出\n");
            pthread_exit(NULL);
        }
        taskFlag = taskDeQueue(pQue, &pnew);
        pthread_cleanup_pop(1); // 非零，则立即执行该清理处理程序；如果为零，则仅弹出而不执行。用来取代解锁行为，同时当使用pthread_exit时会执行之前push的清理函数
        if(!taskFlag){  // 拿到任务
            printf("开始处理客户端的各种请求\n");
            mysql_connet(&conn);
            client_handle(pnew, conn);
            mysql_close(conn);
            close(pnew->new_fd);
            free(pnew);
        }
        if(pFactory->exitFlag){ // 处理做完任务的线程
            printf("子线程退出\n");
            pthread_exit(NULL);
        }
    }
}


void client_handle(task_t *pnew, MYSQL *conn){
    int control_code;
    int new_fd = pnew->new_fd;
    char client_data[2000];
    int ret;
    while(1){
        bzero(client_data, sizeof(client_data));
        ret = recv_protocol(new_fd, &control_code, client_data);
        printf("client_data = %s\n", client_data);
        if(ret == -1){
            printf("客户端断开\n");
            break;//客户端断开
        }
        switch (control_code){
        case LS:
            do_ls(pnew, conn);
            break;
        case CD:
            if(strlen(client_data))
                do_cd(pnew, client_data, conn);
            break;
        case PWD:
            do_pwd(pnew);
            break;
        case MKDIR:
            if(strlen(client_data))
                do_mkdir(pnew, conn, client_data);
            break;
        case RM:
            if(strlen(client_data))
                do_rm(pnew, conn, client_data);
            break;
        case GETS:
            break;
        case PUTS:
            break;
        default:
            break;
        }
    }
}

void do_ls(task_t *pnew, MYSQL *conn){
    send_file_info(pnew, conn);
}

void do_cd(task_t *pnew, char *dir_name, MYSQL *conn){
    if(!strcmp(dir_name, "..")){
        if(pnew->path_id != 0){
            pnew->path_id = get_pre_id(conn, pnew->path_id);
            modify_path_slash(pnew->path);
        }
       send_protocol(pnew->new_fd, SUCCESS, pnew->path, strlen(pnew->path));
    }
    else{
        int path_id = check_cd_dir_isexist(conn, pnew->user_id, pnew->path_id, dir_name);
        if(path_id){
            pnew->path_id = path_id;
            char new_path[2000];
            sprintf(new_path, "%s%s/", pnew->path, dir_name);
            strcpy(pnew->path, new_path);
            send_protocol(pnew->new_fd, SUCCESS, pnew->path, strlen(pnew->path));  
        }
        else{
            send_protocol(pnew->new_fd, FAILED, NULL, 0);
        }
    }
}

void do_pwd(task_t *pnew){
    send_protocol(pnew->new_fd, SUCCESS, pnew->path, strlen(pnew->path));
}

void do_mkdir(task_t *pnew, MYSQL *conn, char *dir_name){
    // 查询当前目录是否有同名的目录
    int dir_isexist;
    dir_isexist = check_dir_isexist(conn,pnew->user_id,pnew->path_id,dir_name);
    if(dir_isexist)//存在同名文件，不能创建
    {
        send_protocol(pnew->new_fd,FAILED,NULL,0);
    }else{
        int ret = creat_dir(conn,pnew->user_id,pnew->path_id,dir_name);
        if(ret)
            send_protocol(pnew->new_fd,SUCCESS,NULL,0);
        else
            send_protocol(pnew->new_fd,FAILED,NULL,0);
    }
}
void do_rm(task_t *pnew, MYSQL *conn, char* rm_file_name){
    // 查询是否有这个文件名，如果有就删除，如果没有就什么都不做
    // 目前能删除的只有空文件夹和文件
    int ret = rm_file_emptydir(pnew, conn, rm_file_name);    
    if(ret){
        send_protocol(pnew->new_fd, SUCCESS, NULL, 0);
    }
    else{
        send_protocol(pnew->new_fd, FAILED, NULL, 0);
    }
}

void do_puts(task_t *pnew){

}

void do_gets(task_t *pnew){

}
