#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/types.h>
#include <linux/netfilter.h>
#include <libnetfilter_queue/libnetfilter_queue.h>
#include <error.h>
#include <errno.h>
#include "lib.h"

#define ADDR_SIZE 1000
#define CONF_TIMEOUT 10
#define OWNER 1

int cb(struct nfq_q_handle *qh,struct nfgenmsg *nfmsg,struct nfq_data *nfa,void *data);
// u_int32_t print_pkt (struct nfq_data *tb);

int def;

int main() {
	struct nfq_handle *h;
	struct nfq_q_handle *qh;
	struct nfnl_handle *nh;
	int fd;
	int rv;
	char buf[4096];
	
	struct addr_policy *pol[ADDR_SIZE];
	int i;
	char *addr = "192.168.1.69";
	unsigned int configured;
	
	if((def = read_net_policies(addr,81,OWNER,pol,ADDR_SIZE)) < 0) error(1,errno,"read_policies");
	configured = get_time();
	
	if((h = nfq_open()) == NULL) error(1,errno,"nfq_open");
	if(nfq_unbind_pf(h,AF_INET) < 0) error(1,errno,"nfq_unbind_pf");
	if(nfq_bind_pf(h,AF_INET) < 0) error(1,errno,"nfq_bind_pf");
	
	if((qh = nfq_create_queue(h,  0, &cb, pol)) == NULL) error(1,errno,"nfq_create_queue");
	if(nfq_set_mode(qh, NFQNL_COPY_PACKET, 0xffff) < 0) error(1,errno,"nfq_set_mode");
	fd = nfq_fd(h);
	while ((rv = recv(fd, buf, sizeof(buf), 0)) && rv >= 0) {
		if(get_time()+(CONF_TIMEOUT*60) <= configured) {
			printf("Reconfiguring\n");
			if((def = read_net_policies(addr,81,OWNER,pol,ADDR_SIZE)) < 0) error(1,errno,"read_policies");
			configured = get_time();
		}
		nfq_handle_packet(h, buf, rv);
	}
	if(nfq_close(h) < 0) error(1,errno,"nfq_close");
	return 0;
}

int cb(struct nfq_q_handle *qh,struct nfgenmsg *nfmsg,struct nfq_data *nfa,void *data) {
	struct nfqnl_msg_packet_hw *hwph;
	struct nfqnl_msg_packet_hdr *ph;
	unsigned char *buff[1];
	unsigned char addr[4] = {8,8,8,8};
	int id = 0;
	// u_int32_t id = print_pkt(nfa);
	if (ph = nfq_get_msg_packet_hdr(nfa)) id = ntohl(ph->packet_id);
	
	if(nfq_get_payload(nfa,buff) >= 0) if(memcmp(buff[0]+16,addr,4) == 0) return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
	
	if (hwph = nfq_get_packet_hw(nfa)) {
		// int hlen = ntohs(hwph->hw_addrlen);
		return nfq_set_verdict(qh, id, get_policy(hwph->hw_addr,(struct addr_policy **)data,def), 0, NULL);
	}
	return nfq_set_verdict(qh, id, NF_ACCEPT, 0, NULL);
}

// u_int32_t print_pkt (struct nfq_data *tb) {
	// int id = 0;
	// struct nfqnl_msg_packet_hdr *ph;
	// struct nfqnl_msg_packet_hw *hwph;
	// u_int32_t mark,ifi; 
	// int ret;
	// unsigned char *data;

	// ph = nfq_get_msg_packet_hdr(tb);
	// if (ph) {
		// id = ntohl(ph->packet_id);
		// printf("hw_protocol=0x%04x hook=%u id=%u ",ntohs(ph->hw_protocol), ph->hook, id);
	// }

	// hwph = nfq_get_packet_hw(tb);
	// if (hwph) {
		// int i, hlen = ntohs(hwph->hw_addrlen);
		// printf("hw_src_addr=");
		// for (i = 0; i < hlen-1; i++) printf("%02x:", hwph->hw_addr[i]);
		// printf("%02x ", hwph->hw_addr[hlen-1]);
	// }

	// mark = nfq_get_nfmark(tb);
	// if (mark) printf("mark=%u ", mark);

	// ifi = nfq_get_indev(tb);
	// if (ifi) printf("indev=%u ", ifi);

	// ifi = nfq_get_outdev(tb);
	// if (ifi) printf("outdev=%u ", ifi);

	// ifi = nfq_get_physindev(tb);
	// if (ifi) printf("physindev=%u ", ifi);

	// ifi = nfq_get_physoutdev(tb);
	// if (ifi) printf("physoutdev=%u ", ifi);

	// ret = nfq_get_payload(tb, &data);
	// if (ret >= 0) printf("payload_len=%d ", ret);

	// fputc('\n', stdout);

	// return id;
// }