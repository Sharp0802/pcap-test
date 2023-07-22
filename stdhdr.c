#include "stdhdr.h"
#include <stdio.h>

#ifdef __whopr__
__attribute__((externally_visible))
#endif
void prt_mac(mac_t mac)
{
	printf("%02hhX:%02hhX:%02hhX:%02hhX:%02hhX:%02hhX", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

#ifdef __whopr__
__attribute__((externally_visible))
#endif
void prt_ip(ipv4_t ip, uint16_t port)
{
	printf("%hhu.%hhu.%hhu.%hhu:%hu", ip[0], ip[1], ip[2], ip[3], port);
}
