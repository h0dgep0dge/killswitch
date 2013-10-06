/***************************************************************\
  A library that provides simple functions to support filtering
\***************************************************************/

#include "lib.h"
#include <stdio.h>
#include <ctype.h>

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
		if(sscanf(buff,"default=%d",&def) > 0);
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
