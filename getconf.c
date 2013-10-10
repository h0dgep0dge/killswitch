#include <stdio.h>#include <sys/types.h>#include <sys/socket.h>#include <error.h>#include <errno.h>

int main() {
	int sock;	struct sockaddr_in bind_addr;	char buff[0xfff];		if((sock = socket(AF_INET,SOCK_STREAM,0)) < 0) error(1,errno,"socket");		memset(&bind_addr,0,sizeof(struct sockaddr_in));	bind_addr.sin_family = AF_INET;	bind_addr.sin_port = htons(1500);	bind_addr.sin_addr.s_addr = INADDR_ANY;		if(bind(sock,(struct sockaddr *)&bind_addr,sizeof(struct sockaddr_in)) < 0) error(1,errno,"bind");
	return 0;
}