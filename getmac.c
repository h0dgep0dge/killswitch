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
	int sock,l;
	int addrl = sizeof(struct sockaddr_in);
	struct sockaddr_in bind_addr;
	struct sockaddr_in *p;
	struct sockaddr_in cli_addr;
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
	
	while((l = recvfrom(sock,buff,0xffff,0,(struct sockaddr *)&cli_addr,&addrl)) >= 0) {
		int r;
		sendto(sock,"SCANNING",8,0,(struct sockaddr *)&cli_addr,addrl);
		memset(&request,0,sizeof(struct arpreq));
		memcpy(&(request.arp_pa),&cli_addr,sizeof(struct sockaddr));
		memcpy(&(request.arp_netmask),&netmask,sizeof(struct sockaddr));
		strcpy(request.arp_dev,"eth0");
		if(ioctl(sock,SIOCGARP,&request) < 0) error(1,errno,"Ioctl");
		// printf("%.2x:%.2x:%.2x:%.2x:%.2x:%.2x \n",(unsigned char)request.arp_ha.sa_data[0],(unsigned char)request.arp_ha.sa_data[1],
												  // (unsigned char)request.arp_ha.sa_data[2],(unsigned char)request.arp_ha.sa_data[3],
												  // (unsigned char)request.arp_ha.sa_data[4],(unsigned char)request.arp_ha.sa_data[5]);
		sendto(sock,request.arp_ha.sa_data,6,0,(struct sockaddr *)&cli_addr,addrl);
	}
	return 0;
}