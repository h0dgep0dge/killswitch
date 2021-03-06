/***************************************************************\
  A library that provides simple functions to support filtering
\***************************************************************/

#include "lib.h"
#include <stdio.h>
#include <ctype.h>
#include <error.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <string.h>

unsigned int get_policy(void *addr,struct addr_policy **policies,unsigned int def) {
	int i;
	for(i = 0;policies[i];i++) if(memcmp(addr,&(policies[i]->client_addr),6) == 0) return policies[i]->policy;
	return def;
}

int read_policies(const char *filename,struct addr_policy **policies,size_t pol_len) {
	char buff[500];
	FILE *strm;
	int def = 0,r,overrun=0,i;
	int p_offset = 0;
	policies[0] = NULL;
	int line = 1;
	if((strm = fopen(filename,"r")) == NULL) return -1;
	while(fgets(buff,500,strm)) {
		unsigned int mac[6];
		unsigned char mac_c[6];
		int pol;
		if(buff[0] == '#');
		else if(sscanf(buff,"default=%d",&def) > 0);
		else if((r = sscanf(buff,"client={%x:%x:%x:%x:%x:%x,%d}",mac,mac+1,mac+2,mac+3,mac+4,mac+5,&pol)) > 0) {
			if(r != 7) fprintf(stderr,"Malformed client on line %i \n",line);
			else if(p_offset < pol_len-1) {
				if((policies[p_offset] = malloc(sizeof(struct addr_policy))) == NULL) return -1;
				policies[p_offset+1] = NULL;
				policies[p_offset]->policy = pol;
				for(i = 0;i < 6;i++) policies[p_offset]->client_addr[i] = (unsigned char)mac[i];
				p_offset++;
			} else if(overrun == 0) {
				fprintf(stderr,"Policy vector overrun \n");
				overrun = 1;
			}
		}
		else {
			fprintf(stderr,"Invalid line %i \n",line);
		}
		line++;
	}
	fclose(strm);
	return def;
}

int read_net_policies(const char *addr,unsigned short port,unsigned int owner,struct addr_policy **policies,size_t pol_len) {
	char buff[500];
	FILE *strm;
	int def = 0,r,overrun=0,i;
	int p_offset = 0;
	policies[0] = NULL;
	int line = 1;
	int sock;
	struct sockaddr_in bind_addr;
	
	if((sock = socket(AF_INET,SOCK_STREAM,0)) < 0) return -1;
	
	memset(&bind_addr,0,sizeof(struct sockaddr_in));
	bind_addr.sin_family = AF_INET;
	bind_addr.sin_port = htons(port);
	if(inet_aton(addr,&(bind_addr.sin_addr)) == 0) {
		errno = EFAULT;
		return -1;
	}
	if(connect(sock,(struct sockaddr *)&bind_addr,sizeof(struct sockaddr_in)) < 0) return -1;
	
	if((strm = fdopen(sock,"r+")) == NULL) return -1;
	fprintf(strm,"%i\n",owner);
	while(fgets(buff,500,strm)) {
		unsigned int mac[6];
		unsigned char mac_c[6];
		int pol;
		if(buff[0] == '#');
		else if(sscanf(buff,"default=%d",&def) > 0);
		else if((r = sscanf(buff,"client={%x:%x:%x:%x:%x:%x,%d}",mac,mac+1,mac+2,mac+3,mac+4,mac+5,&pol)) > 0) {
			if(r != 7) fprintf(stderr,"Malformed client on line %i \n",line);
			else if(p_offset < pol_len-1) {
				if((policies[p_offset] = malloc(sizeof(struct addr_policy))) == NULL) return -1;
				policies[p_offset+1] = NULL;
				policies[p_offset]->policy = pol;
				for(i = 0;i < 6;i++) policies[p_offset]->client_addr[i] = (unsigned char)mac[i];
				p_offset++;
			} else if(overrun == 0) {
				fprintf(stderr,"Policy vector overrun \n");
				overrun = 1;
			}
		}
		else {
			fprintf(stderr,"Invalid line %i \n",line);
		}
		line++;
	}
	fclose(strm);
	return def;
}

time_t get_lwrite(char *filename) {
	struct stat info;
	if(stat(filename,&info) < 0) error(1,errno,"stat");
	return info.st_mtime;
}

void err(const char *messg,int status) {
	fprintf(stderr,"%s \n",messg);
	if(status > 0) exit(status);
}

time_t get_time() {
	struct timeval time;
	memset(&time,0,sizeof(struct timeval));
	if(gettimeofday(&time,NULL) < 0) return -1;
	return time.tv_sec;
}