#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/types.h>
#include <linux/netfilter.h>
#include <libnetfilter_queue/libnetfilter_queue.h>
#include <error.h>
#include <errno.h>

int main() {
	struct nfq_handle *h;
	
	if((h = nfq_open()) == NULL) error(1,errno,"nfq_open");
	if (nfq_unbind_pf(h,AF_INET) < 0) error(1,errno,"nfq_unbind_pf");
	if (nfq_bind_pf(h,AF_INET) < 0) error(1,errno,"nfq_bind_pf");
	printf("JAMES IS A DICK LAWL \n");
	if (nfq_close(h) < 0) error(1,errno,"nfq_close");
	return 0;
}