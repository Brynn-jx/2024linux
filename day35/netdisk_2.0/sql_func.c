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