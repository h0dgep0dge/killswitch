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
	int def,i;
	struct addr_policy **p;
	if((def = read_policies("./filter.conf",pol,1000)) < 0) error(1,errno,"read_policies");
	for(p = pol;*p;p++) {
		for(i = 0;i < 6;i++) printf("%x:",(unsigned char)(*p)->client_addr[i]);
		printf(", %i \n",(*p)->policy);
	}
	return 0;
}