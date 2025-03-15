#include "factory.h"

// 初始化mysql链接
int mysql_connet(MYSQL **conn){
    const char* server="192.168.88.128";
	const char* user="root";
	const char* password="123";
	const char* database="netdisk";//要访问的数据库名称
	*conn=mysql_init(NULL);//要初始化
    if(mysql_real_connect(*conn, server, user, password, database, 0, NULL, 0)){
        printf("Connected...\n"); 
        return 0;
    }
    else{
        printf("Error connecting to database:%s\n",mysql_error(*conn));
		return -1;
    }
}

// 查询用户名是否存在
int check_username_exist(MYSQL *conn, char *username){
    int ret, ret_flag;
    char *p = "SELECT user_name FROM users WHERE user_name = '";
    char query[300] = {0};
    sprintf(query, "%s%s'", p, username);
    puts(query);
    MYSQL_RES* res;
    MYSQL_ROW row;
    ret = mysql_query(conn, query); // 成功时返回0. 不成功时返回错误码
    if(ret){
        printf("Error making query:%s\n",mysql_error(conn));//sql的语法出错
    }else{
        printf("Query made...\n");
		res=mysql_use_result(conn);//获取结果
        if(res){
            row = mysql_fetch_row(res);
            if(row == NULL){
                ret_flag = 0;   // 没查询到结果
            }else{
                ret_flag = 1;
            }
        }
        mysql_free_result(res);
    }
    return ret_flag;
}

// 插入用户
uint64_t insert_new_user(MYSQL *conn, user_passwd_t *user_passwd){
    char *p = "INSERT INTO users VALUES (0, '";
    char query[1000] = {0};
    char salt[20]="$6$";
	generate_random_string(salt);//往后拼接16个随机salt
    struct crypt_data cr_data;
	crypt_r(user_passwd->password,salt,&cr_data);//得到密文 cr_data.output这里存放密文 线程安全函数
    sprintf(query, "%s%s','%s', '%s')", p, user_passwd->username, salt, cr_data.output);
    puts(query);
    int ret = mysql_query(conn, query);
    if(ret){    // 没成功
        printf("Error making query:%s\n",mysql_error(conn));
		return -1;
    }   
    else{
        return mysql_affected_rows(conn);
    }
}

// 检查用户名，密码是否存在
int check_login(MYSQL *conn, user_passwd_t *user_passwd){
    char *p = "SELECT salt, ciphertext FROM users WHERE user_name = '";
    char query[300] = {0};
    sprintf(query, "%s%s'", p, user_passwd->username);
#ifdef DEBUG
    puts(query);
#endif
    MYSQL_RES *res;
    MYSQL_ROW row;
    struct crypt_data cr_data;
    int login_flag;

    int ret = mysql_query(conn, query);
    if(ret){
        printf("Error making query:%s\n",mysql_error(conn));//sql的语法出错
       
    }else{
        printf("Query made...\n");
        res = mysql_use_result(conn);
        if(res){
            row = mysql_fetch_row(res);
            if(row){
                crypt_r(user_passwd->password, row[0], &cr_data);   // row[0]是盐值
                if(!strcmp(cr_data.output, row[1])){
                    login_flag=1;//登录成功
                }else{
                    login_flag = 0; // 有这个人但是密码错误
                }
            }
            else{
                login_flag = 0; // 没有这个人
            }
        }
        mysql_free_result(res);
    }
    return login_flag;
}

// 返回用户名的id
int get_user_id(MYSQL *conn, char* username){
    char *p = "SELECT user_id FROM users WHERE user_name = '";
    char query[300] = {0};
    sprintf(query, "%s%s'", p, username);
#ifdef DEBUG
    puts(query);
#endif
    MYSQL_RES *res;
    MYSQL_ROW row;
    int user_id;

    int ret = mysql_query(conn, query);
    if(ret){
        printf("Error making query:%s\n",mysql_error(conn));//sql的语法出错
       
    }else{
        printf("Query made...\n");
        res = mysql_use_result(conn);
        if(res){
            row = mysql_fetch_row(res);
            if(row != NULL){
                user_id = atoi(row[0]);
            }
            else{
                user_id = 0;
            }
        }
        mysql_free_result(res);
    }
    return user_id;
}

int check_dir_isexist(MYSQL* conn,int user_id,int path_id,char* dir_name){
    char *p = "SELECT count(*) FROM file_system WHERE user_id = ";
    char query[300] = {0};
    sprintf(query, "%s%d and pre_id = %d and file_name = '%s'", p, user_id, path_id, dir_name);
#ifdef DEBUG
    puts(query);
#endif
    MYSQL_RES *res;
    MYSQL_ROW row;
    int dir_isexist;

    int ret = mysql_query(conn, query);
    if(ret){
        printf("Error making query:%s\n",mysql_error(conn));//sql的语法出错
       
    }else{
        printf("Query made...\n");
        res = mysql_use_result(conn);
        if(res){
            row = mysql_fetch_row(res);
            if(row != NULL){
                dir_isexist = atoi(row[0]);
            }
            else{
                dir_isexist = 0;
            }
        }
        mysql_free_result(res);
    }
    return dir_isexist;
}

int creat_dir(MYSQL* conn,int user_id,int pre_id,char* dir_name){
    char *p = "INSERT INTO file_system (pre_id, file_name, user_id, file_type, file_size) VALUES (";
    char query[1000] = {0};
    sprintf(query, "%s%d,'%s', %d, 'd', 4096)", p, pre_id, dir_name, user_id);
    int ret = mysql_query(conn, query);
    if(ret){    // 没成功
        printf("Error making query:%s\n",mysql_error(conn));
		return 0;
    }   
    else{
        return 1;
    }
}

int get_pre_id(MYSQL *conn, int path_id){
    char *p = "SELECT pre_id FROM file_system WHERE path_id = ";
    char query[300] = {0};
    sprintf(query, "%s%d", p, path_id);
#ifdef DEBUG
    puts(query);
#endif
    MYSQL_RES *res;
    MYSQL_ROW row;
    int pre_id;

    int ret = mysql_query(conn, query);
    if(ret){
        printf("Error making query:%s\n",mysql_error(conn));//sql的语法出错
       
    }else{
        printf("Query made...\n");
        res = mysql_use_result(conn);
        if(res){
            row = mysql_fetch_row(res);
            if(row != NULL){
                pre_id = atoi(row[0]);
            }
            else{
                pre_id = 0;
            }
        }
        mysql_free_result(res);
    }
    return pre_id;
}

// 查询目录的id号
int check_cd_dir_isexist(MYSQL* conn,int user_id,int path_id,char* dir_name){
    char *p = "SELECT path_id FROM file_system WHERE user_id = ";
    char query[300] = {0};
    sprintf(query, "%s%d and pre_id = %d and file_name = '%s'", p, user_id, path_id, dir_name);
#ifdef DEBUG
    puts(query);
#endif
    MYSQL_RES *res;
    MYSQL_ROW row;
    int dir_isexist;

    int ret = mysql_query(conn, query);
    if(ret){
        printf("Error making query:%s\n",mysql_error(conn));//sql的语法出错
       
    }else{
        printf("Query made...\n");
        res = mysql_use_result(conn);
        if(res){
            row = mysql_fetch_row(res);
            if(row != NULL){
                dir_isexist = atoi(row[0]);
            }
            else{
                dir_isexist = 0;
            }
        }
        mysql_free_result(res);
    }
    return dir_isexist;
}


void send_file_info(task_t* pnew, MYSQL *conn) {
    char *p = "SELECT file_type, file_name, file_size FROM file_system WHERE user_id = ";
    char query[300] = {0};
    train_t train;
    sprintf(query, "%s%d and pre_id = %d", p, pnew->user_id, pnew->path_id);
#ifdef DEBUG
    puts(query);
#endif
    MYSQL_RES *res;
    MYSQL_ROW row;
    int dir_isexist;

    int ret = mysql_query(conn, query);
    if(ret){
        printf("Error making query:%s\n",mysql_error(conn));//sql的语法出错
    }
    else{
        printf("Query made...\n");
        res = mysql_use_result(conn);
        if(res){
           while(row = mysql_fetch_row(res)){
                bzero(&train, sizeof(train_t));
                sprintf(train.buf, "%5s%15s%10s", row[0], row[1], row[2]);
                train.size = strlen(train.buf);
                send(pnew->new_fd, &train, train.size + 4, 0);
           }
           //发送个空火车表示结束
           int end_flag = 0;
           send(pnew->new_fd, &end_flag, 4, 0);
        }
        else{
            printf("don't find data\n");
        }
        mysql_free_result(res);
    }
}

// 删除空文件夹和文件
int rm_file_emptydir(task_t* pnew, MYSQL *conn, char* rm_file_name){
    char *p = "select count(*) from file_system where pre_id=(select path_id from file_system where pre_id=";
    char query[2000] = {0};
    sprintf(query, "%s%d and file_name = '%s' and user_id = %d)", p, pnew->path_id, rm_file_name, pnew->user_id);
    puts(query);
#ifdef DEBUG
    puts(query);
#endif
    MYSQL_RES *res;
    MYSQL_ROW row;
    int dir_file_isexist;

    int ret = mysql_query(conn, query);
    if(ret){
        printf("Error making query:%s\n",mysql_error(conn));//sql的语法出错
       
    }else{
        printf("Query made...\n");
        res = mysql_use_result(conn);
        if(res){
            row = mysql_fetch_row(res);
            if(row != NULL){
                dir_file_isexist = atoi(row[0]);
            }
            else{
                dir_file_isexist = 0;   // 空目录或者是文件
            }
        }
        mysql_free_result(res);
    }

    //判断删除的是否是普通文件类型
	char md5_str[50]={0};
	p="select file_type,md5 from file_system where pre_id=";
    bzero(query,sizeof(query));
    sprintf(query, "%s%d and path_name = '%s' and user_id = %d", p, pnew->path_id, rm_file_name, pnew->user_id);
    ret = mysql_query(conn, query);
    if(ret){
        printf("Error making query:%s\n",mysql_error(conn));
    }
    else{
        res = mysql_use_result(conn);
        if(res){
            row = mysql_fetch_row(res);
            if(row[0][0] == 'f'){
                strcpy(md5_str, row[1]);
            }
        }
        mysql_free_result(res);
    }

    if(!dir_file_isexist){
        p="delete from file_system where pre_id=";
        bzero(query, sizeof(query));
        sprintf(query, "%s%d and file_name = '%s' and user_id = %d", p, pnew->path_id, rm_file_name, pnew->user_id);
        int ret = mysql_query(conn, query);
        if(ret){
            printf("Error making query:%s\n",mysql_error(conn));
		}
        else{
			printf("delete success,delete row=%ld\n",(long)mysql_affected_rows(conn));
		}
    }

    // 将引用计数减一
    if(md5_str[0]){
        p="update file_md5sum set reference_count=reference_count-1 where md5='";
		bzero(query,sizeof(query));
		sprintf(query,"%s%s'",p,md5_str);
        ret = mysql_query(conn, query);
        if(ret){
            printf("Error making query:%s\n",mysql_error(conn));
        }
        else{
			printf("delete success,delete row=%ld\n",(long)mysql_affected_rows(conn));
        }
    }
    return !dir_file_isexist;
}


// 查询数据库中是否存在对应的md5
uint64_t check_file_md5_exist(task_t *pnew, MYSQL *conn, char *md5_str){
    char *p = "UPDATE file_md5sum set reference_count = reference_count + 1 WHERE md5 = '";
    char query[300]= {0};
    sprintf(query, "%s%s'", p, md5_str);
    int ret = mysql_query(conn, query);
    if(ret)
	{
		printf("Error making query:%s\n",mysql_error(conn));
		return 0;
	}else{
		return mysql_affected_rows(conn);//存在，这个值是1，不存在这个是0
	}
}

// file_system插入一条数据
uint64_t creat_file(task_t *pnew, MYSQL *conn, char* file_name, char *md5_str){
    char *p = "INSERT INTO file_system (pre_id, file_name, user_id, file_type, md5, file_size) VALUES (";
    char query[1000] = {0};
    sprintf(query, "%s%d,'%s', %d, 'f', '%s', (select file_size from file_md5sum where md5='%s'))", p, pnew->path_id, file_name, pnew->user_id, md5_str, md5_str);
    int ret = mysql_query(conn, query);
    if(ret){    // 没成功
        printf("Error making query:%s\n",mysql_error(conn));
		return 0;
    }   
    else{
        return mysql_affected_rows(conn);
    }
}


uint64_t creat_file_md5(MYSQL *conn, char *md5_str, off_t file_size){
    char *p = "INSERT INTO file_md5sum (md5, reference_count, file_size) VALUES (";
    char query[1000] = {0};
    sprintf(query, "%s'%s', 1, %ld)", p, md5_str, file_size);
    int ret = mysql_query(conn, query);
    if(ret){    // 没成功
        printf("Error making query:%s\n",mysql_error(conn));
		return 0;
    }   
    else{
        return mysql_affected_rows(conn);
    }
}

// 获得文件的md5和文件大小
void get_md5_and_file_size(task_t *pnew, MYSQL *conn, char* md5_str, off_t *file_size, char *file_name_recv_size){
    char *p = "SELECT file_size, md5 FROM file_system WHERE user_id = ";
    char query[300] = {0};
    sprintf(query, "%s%d and pre_id = %d and file_name = '%s'", p, pnew->user_id, pnew->path_id, file_name_recv_size);
    MYSQL_RES *res;
    MYSQL_ROW row;
    int ret = mysql_query(conn, query);
    if(ret){    // 没成功
        printf("Error making query:%s\n",mysql_error(conn));
    }
    else{
        res = mysql_use_result(conn);
        if(res){
            row = mysql_fetch_row(res);
            *file_size = atol(row[0]);
            strcpy(md5_str, row[1]);
        }
    }
}