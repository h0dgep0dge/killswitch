#include <mysql.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

main() {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char *server = "localhost";
	char *user = "root";
	char *password = "-"; /* set me first */
	char *database = "killswitch";
	conn = mysql_init(NULL);
	/* Connect to database */
	if (!mysql_real_connect(conn,server,user,password,database,0,NULL,0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	/* send SQL query */
	if (mysql_query(conn, "SELECT * FROM `policies`")) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	res = mysql_use_result(conn);
	/* output table name */
	printf("MySQL Tables in mysql database:\n");
	while ((row = mysql_fetch_row(res)) != NULL) printf("client={%s,%s} \n",row[2],row[3]);
	/* close connection */
	mysql_free_result(res);
	mysql_close(conn);
}