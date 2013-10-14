#include <mysql.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "lib.h"
#include "creds.c"

main() {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char *server = "localhost";
	char *database = "killswitch";
	conn = mysql_init(NULL);
	if(conn == NULL) err("Failed to create conn",1);
	
	if (!mysql_real_connect(conn,server,user,password,database,0,NULL,0)) err(mysql_error(conn),1);
	
	if (mysql_query(conn, "SELECT * FROM `policies`")) err(mysql_error(conn),1);
	res = mysql_use_result(conn);
	
	while ((row = mysql_fetch_row(res)) != NULL) printf("client={%s,%s} \n",row[2],row[3]);

	mysql_free_result(res);
	mysql_close(conn);
}