#ifndef PCAP_TEST_STDHDR_H
#define PCAP_TEST_STDHDR_H

#include <stdint.h>
#include <sys/types.h>

typedef uint8_t mac_t[6];
typedef uint8_t ipv4_t[4];

typedef struct
{
    mac_t	dmac;
    mac_t       smac;
    uint16_t    type;
} eth_hdr_t;

typedef struct
{
    uint8_t     ver     : 4;
    uint8_t     ihl     : 4;
    uint8_t     dscp    : 6;
    uint8_t     ecn     : 2;
    uint16_t    len;
    
    uint16_t    id;
    uint16_t    flags   : 3;
    uint16_t    fragofs : 13;
    
    uint8_t     ttl;
    uint8_t     prtcl;
    uint16_t    chksum;

    ipv4_t      sip;
    ipv4_t      dip;
} ipv4_hdr_t;

typedef struct
{
    uint16_t    sport;
    uint16_t    dport;
    
    uint32_t    seqno;
    uint32_t    ackno;

    uint8_t     datofs  : 4;
    uint8_t     rsrvd   : 4;
    uint8_t     flags;
    uint16_t    wndsiz;
    
    uint16_t    chksum;
    uint16_t    urgptr;
} tcp_hdr_t;

void prt_mac(mac_t mac);
void prt_ip(ipv4_t ip, uint16_t port);

#endif // PCAP_TEST_STDHDR_H
