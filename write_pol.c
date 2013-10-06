#include <stdlib.h>
#include <linux/ip.h>
#include <netinet/in.h>
#include <linux/netfilter.h>
#include <libipq.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include "lib.h"

int main() {
	struct addr_policy *pol[1000];
	struct addr_policy robbie;
	unsigned char mac[6] = {136,159,250,128,243,163};
	int i = 0;
	memset(&robbie,0,sizeof(struct addr_policy));
	memcpy(robbie.client_addr,mac,6);
	robbie.policy = NF_ACCEPT;
	pol[i++] = &robbie;
	pol[i] = NULL;
	write_policies("./pol_dump",pol,1000);
	printf("%i \n",sizeof(robbie.client_addr));
	return 0;
}