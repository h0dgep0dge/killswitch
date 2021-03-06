/********************************************************\
* A basic command line tool to easily find a mac address *
\********************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <netinet/if_ether.h>
#include <sys/ioctl.h>
#include "lib.h"

int main(int argc,char **argv) {
	struct arpreq request;
	int sock;
	struct sockaddr_in target;
	if(argc != 2) err("Wrong argument",1);
	
	memset(&target,0,sizeof(struct sockaddr_in));
	target.sin_family = AF_INET;
	inet_aton(argv[1],&(target.sin_addr));
	
	sock = socket(AF_INET,SOCK_DGRAM,0);
	memset(&request,0,sizeof(struct arpreq));
	memcpy(&(request.arp_pa),&target,sizeof(struct sockaddr));
	strcpy(request.arp_dev,"eth0");
	if(ioctl(sock,SIOCGARP,&request) < 0) {
		// if(errno == 6) {
			// printf("ERROR\n");
			// return 1;
		// }
		error(1,errno,"Ioctl %i",errno);
	}
	printf("%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",(unsigned char)request.arp_ha.sa_data[0],(unsigned char)request.arp_ha.sa_data[1],
											 (unsigned char)request.arp_ha.sa_data[2],(unsigned char)request.arp_ha.sa_data[3],
											 (unsigned char)request.arp_ha.sa_data[4],(unsigned char)request.arp_ha.sa_data[5]);
	return 0;
}
