#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
int main(int argc,char* argv[])
{
	MYSQL *conn;//连接对象
   // MYSQL_RES *res;
   // MYSQL_ROW row;
	const char* server="localhost";
	const char* user="root";
	const char* password="123";
	const char* database="test";
	char query[200]="INSERT INTO pet(name,OWNER,birth) VALUES\
	('xiongda','luke1','2008-11-01');";
	int t;
	conn=mysql_init(NULL);//初始化
	//mysql_real_connect失败返回0
	if(!mysql_real_connect(conn,server,user,password,database,0,NULL,0))
	{
		printf("Error connecting to database:%s\n",mysql_error(conn));
		goto end;
	}else{
		printf("Connected...\n");
	}
	t=mysql_query(conn,query);//执行sql
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(conn));
	}else{
		printf("insert success,insert row=%ld\n",(long)mysql_affected_rows(conn));
	}
end:
	mysql_close(conn);
	return 0;
}
