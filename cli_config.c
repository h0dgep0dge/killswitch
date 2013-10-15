#include <mysql.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "lib.h"
#include "creds.c" // So i don't have to post my creds on the internet.

int create_sock(int type,unsigned short port,int backlog);
int handle_client(int sock);

int main() {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char *server = "localhost";
	char *database = "killswitch";
	int sock,c_sock;
	struct sockaddr_in bind_addr;
	conn = mysql_init(NULL);
	if(conn == NULL) err("Failed to create conn",1);
	if (!mysql_real_connect(conn,server,user,password,database,0,NULL,0)) err(mysql_error(conn),1);
	
	if((sock = create_sock(SOCK_STREAM,81,10)) < 0) error(1,errno,"create_sock");
	while((c_sock = accept(sock,NULL,NULL)) > 0) {
		int r = fork();
		if(r == 0) {
			handle_client(c_sock);
			return 0;
		}
		else if(r < 0) error(1,errno,"fork");
		close(c_sock);
	}
	return 0;
	if (mysql_query(conn, "SELECT * FROM `policies`")) err(mysql_error(conn),1);
	res = mysql_use_result(conn);
	
	while ((row = mysql_fetch_row(res)) != NULL) printf("client={%s,%s} \n",row[2],row[3]);

	mysql_free_result(res);
	mysql_close(conn);
	return 0;
}

int create_sock(int type,unsigned short port,int backlog) {
	int sock;
	struct sockaddr_in bind_addr;
	
	memset(&bind_addr,0,sizeof(struct sockaddr_in));
	bind_addr.sin_family = AF_INET;
	bind_addr.sin_port = htons(port);
	bind_addr.sin_addr.s_addr = INADDR_ANY;
	
	if((sock = socket(AF_INET,type,0)) < 0) return -1;
	if(bind(sock,(struct sockaddr *)&bind_addr,sizeof(struct sockaddr_in)) < 0) return -1;
	if(listen(sock,backlog) < 0) return -1;
	return sock;
}

int handle_client(int sock) {
	FILE *sock_str;
	char buf[50];
	int owner,i;
	sock_str = fdopen(sock,"w+");
	
	fgets(buf,50,sock_str);
	for(i = 0;i < 50 && buf[i] != '\0';i++) if(!isspace(buf[i]) && !isdigit(buf[i])) {
		fprintf(sock_str,"BROKEN INPUT\n");
		fclose(sock_str);
		return -1;
	}
	owner = atoi(buf);
	fprintf(sock_str,"%i \n",owner);
	fclose(sock_str);
	return 0;
}