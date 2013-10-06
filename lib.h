/***************************************************************************\
  Headers for a library that provides simple functions to support filtering
\***************************************************************************/

#include <stdlib.h>

struct addr_policy {
	char client_addr[6];
	unsigned int policy;
};

unsigned int get_policy(void *addr,struct addr_policy **policies,unsigned int def);
int read_policies(const char *filename,struct addr_policy **policies,size_t pol_len);