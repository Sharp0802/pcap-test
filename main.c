#include <pcap.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "stdhdr.h"
#include "log.h"


void prt_usage()
{
	printf("syntax: pcap-test <interface>\n");
	printf("sample: pcap-test wlan0\n");
}

typedef struct 
{
	char* dev;
} param_t;

bool parse_args(param_t* dst, int32_t argc, char* argv[])
{
	if (argc != 2)
		return false;

	dst->dev = argv[1];
	return true;
}

void sigint_handler(int _)
{
	exit(0);
}

int32_t main(int32_t argc, char* argv[]) 
{
	param_t param;
	if (!parse_args(&param, argc, argv))
	{
		prt_usage();
		return -1;
	}

	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t* pcap = pcap_open_live(param.dev, BUFSIZ, 1, 1000, errbuf);
	if (pcap == NULL) 
	{
		prt_err("pcap_open_live(%s) return null - %s", param.dev, errbuf);
		return -1;
	}

	signal(SIGINT, sigint_handler);
	while (true) 
	{
		struct pcap_pkthdr* hdr;
		const u_char* packet;
		int res = pcap_next_ex(pcap, &hdr, &packet);
		if (res == 0) continue;
		if (res == PCAP_ERROR || res == PCAP_ERROR_BREAK) 
		{
			prt_info("pcap_next_ex return %d(%s)\n", res, pcap_geterr(pcap));
			break;
		}

		size_t offset = 0;
		
		eth_hdr_t* peth = (eth_hdr_t*)(packet + offset);
		offset += sizeof(*peth);
		if (offset >= hdr->caplen) continue;
		if (peth->type != 0x08 /* IPv4 */) continue;

		ipv4_hdr_t* pip = (ipv4_hdr_t*)(packet + offset);
		offset += sizeof(*pip);
		if (offset >= hdr->caplen) continue;
		if (pip->prtcl != 0x06 /* TCP */) continue;

		tcp_hdr_t* ptcp = (tcp_hdr_t*)(packet + offset);
		offset += ptcp->datofs;

		printf("====╪= TCP #%05u =======================\n", ntohs(pip->id));

		/* print src addr */
		fputs(" SRC|", stdout);
		prt_mac(peth->smac);
		fputc('-', stdout);
		prt_ip(pip->sip, ntohs(ptcp->sport));
		fputc('\n', stdout);

		/* print dst addr */
		fputs(" DST|", stdout);
		prt_mac(peth->dmac);
		fputc('-', stdout);
		prt_ip(pip->dip, ntohs(ptcp->dport));
		fputc('\n', stdout);

		/* print packet siz */
		printf("SIZE|%uB\n", hdr->caplen);

		/* dump payload */
		fputs("DUMP|", stdout);
		size_t n = hdr->caplen - offset;
		if (n > 10) n = 10;
		for (size_t i = 0; i < n; ++i)
		{
			printf("%02X ", packet[offset + i]);
		}
		fputc('\n', stdout);
	}

	pcap_close(pcap);

	exit(0);
}
