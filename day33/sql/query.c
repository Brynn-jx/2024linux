#include<mysql/mysql.h>
#include<stdio.h>

int main(int argc, char* argv[]){
    if(argc!=2){
		printf("error args\n");
		return -1;
	}
    MYSQL* conn;
    MYSQL_RES* res;
    MYSQL_ROW row;
    const char* server = "192.168.88.128";
    const char* user = "root";
    const char* password = "123";
    const char* database = "test";
    const char* p = "select * from pet where sex=";
    char query[200] = {0};
    sprintf(query, "%s'%s'", p, argv[1]);
    puts(query);
    conn = mysql_init(NULL);
    if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0)){
		printf("Error connecting to database:%s\n",mysql_error(conn));
		return -1;
	}else{
		printf("Connected...\n");
	}
    unsigned int t = mysql_query(conn, query);
    if(t){
        printf("Error making query:%s\n",mysql_error(conn));//sql的语法出错
    }else{
        res = mysql_use_result(conn);
        if(res){
            while(row = mysql_fetch_row(res)){
                for(t = 0; t < mysql_num_fields(res); t++){
                    printf("%10s ",row[t]);
                }
                printf("\n");
            }
        }else{
            printf("Don't find data\n");
        }
        mysql_free_result(res);   
    }
    return 0;
}