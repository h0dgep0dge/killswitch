/**************************************\
*  An ip_queue example program         *
*  Filters IPv4 Packets on HW Address  *
*  By Robbie Mckennie                  *
\**************************************/

#include <stdlib.h>
#include <linux/ip.h>
#include <netinet/in.h>
#include <linux/netfilter.h>
#include <libipq.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "lib.h"

#define ADDR_SIZE 1000

void handler();
time_t get_lwrite(char *filename);

int run;

int main() {
	struct ipq_handle *h;
	struct addr_policy *pol[ADDR_SIZE];
	char *conf_file = "./filter.conf";
	int def;
	int configured = get_lwrite(conf_file);
	if((def = read_policies(conf_file,pol,ADDR_SIZE)) < 0) error(1,errno,"read_policies");
	
	run = 1;
	if((h = ipq_create_handle(0,NFPROTO_IPV4)) == NULL) {
		ipq_perror(ipq_errstr());
		exit(1);
	}
	
	if(ipq_set_mode(h,IPQ_COPY_PACKET,0) < 0) {
		ipq_perror(ipq_errstr());
		exit(1);
	}
	signal(SIGINT,handler);
	while(run) {
		unsigned char buff[2000];
		ipq_packet_msg_t *pkt;
		int l,i;
		l = ipq_read(h,buff,2000,500000);
		if(l <= 0) continue; // err/timeout, doesn't give enough info yet. FIXME
		if(time(NULL) > configured+CONF_TIMEOUT) {
			configured = time(NULL);
			if((def = read_policies("./filter.conf",pol,ADDR_SIZE)) < 0) error(1,errno,"read_policies");
		}
		pkt = ipq_get_packet(buff);
		if(ipq_set_verdict(h,pkt->packet_id,get_policy(pkt->hw_addr,pol,def),0,NULL) < 0) {
			printf("Err \n");
			continue;
		}
	}
	
	if(ipq_destroy_handle(h) < 0) {
		ipq_perror(ipq_errstr());
		exit(1);
	}
	return 0;
}

void handler() {
	printf("QUITTING \n");
	run = 0;
}

time_t get_lwrite(char *filename) {
	struct stat info;
	if(stat(filename,&info) < 0) error(1,errno,"stat");
	return info.st_mtime;
}