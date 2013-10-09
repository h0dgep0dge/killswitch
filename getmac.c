/*************************************************\
* A program to give a lan client it's mac address *
\*************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <netinet/if_ether.h>
#include <sys/ioctl.h>

int main() {
	int sock,l,addrl;
	struct sockaddr_in bind_addr;
	struct sockaddr_in *p;
	struct sockaddr cli_addr;
	struct sockaddr_in netmask;
	struct arpreq request;
	char buff[0xffff];
	
	if((sock = socket(AF_INET,SOCK_DGRAM,0)) < 0) error(1,errno,"socket");
	
	memset(&bind_addr,0,sizeof(struct sockaddr_in));
	bind_addr.sin_family = AF_INET;
	bind_addr.sin_port = htons(50);
	bind_addr.sin_addr.s_addr = INADDR_ANY;
	
	memset(&netmask,0,sizeof(struct sockaddr_in));
	netmask.sin_family = AF_INET;
	inet_aton("255.255.255.255",&(netmask.sin_addr));
	if(bind(sock,(struct sockaddr *)(&bind_addr),sizeof(struct sockaddr_in)) < 0) error(1,errno,"socket");
	
	while((l = recvfrom(sock,buff,0xffff,0,&cli_addr,&addrl)) >= 0) {
		int r;
		memset(&request,0,sizeof(struct arpreq));
		// memcpy(&(request.arp_pa),&cli_addr,sizeof(struct sockaddr));
		p = (struct sockaddr_in *)&(request.arp_pa);
		// p->sin_family = AF_INET;
		// p->sin_addr.s_addr = cli_addr.sin_addr.s_addr;
		// memcpy(&(request.arp_netmask),&netmask,sizeof(struct sockaddr));
		// strcpy(request.arp_dev,"eth0");
		// r = ioctl(sock,SIOCGARP,&request);
		// if(() != 100) error(1,errno,"Ioctl");
		printf("%s %i \n",inet_ntoa(((struct sockaddr_in *)(&cli_addr))->sin_addr),r);
	}
	error(1,errno,"Recv");
	return 0;
}