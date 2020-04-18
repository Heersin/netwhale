#include "parse.h"

int parseIP(u_char *pkt_data)
{
    ip_header *ih;
    u_int ip_len;
    ih = (ip_header *) pkt_data;

    ip_len = (ih->ver_ihl & 0xf) << 2;

    printf("[IP]%d.%d.%d.%d -> %d.%d.%d.%d\n",
        ih->saddr.byte1,
        ih->saddr.byte2,
        ih->saddr.byte3,
        ih->saddr.byte4,
        ih->daddr.byte1,
        ih->daddr.byte2,
        ih->daddr.byte3,
        ih->daddr.byte4);
    
    
    playByte(pkt_data + 0);	// 1st : version and len
    playByte(pkt_data + 1);	// 2nd : service
    playByte(pkt_data + 6);	// 7th : flag and offset
    playByte(pkt_data + 7);		
    playByte(pkt_data + 8);	// ttl
    playByte(pkt_data + 9);	// protocol
    return ip_len;
}

void parseARP(u_char *data)
{

    playByte(data + 0);	// hardware type
    playByte(data + 1);	
    playByte(data + 2);	// proto type
    playByte(data + 3);

    playByte(data + 6);	// op code
    playByte(data + 7);

    printf("ARP OPERATION : 0x%x\n", ntohs(((arp_header *)data)->ar_op));
}


void parseUDP(u_char *data)
{
    udp_header *hdr;
    hdr = (udp_header *)data;

    // length
    playByte(data + 4);
    playByte(data + 5);

    printf("[UDP]Port %d ---> %d\n", ntohs(hdr->sport), ntohs(hdr->dport));
}

int parseTCP(u_char *data)
{
    tcp_header *hdr;
    int hdrlen;
    hdr = (tcp_header *)data;
    
    playByte(data + 12);	// offset
    playByte(data + 13);	// flag
    playByte(data + 14);	// window size
    playByte(data + 15);	
    playByte(data + 17);	// urgent pointer
    
    
    
    printf("[TCP]Port %d ---> %d\n", ntohs(hdr->source), ntohs(hdr->dest));
    hdrlen = (hdr->doff) << 2;
    return hdrlen;
}

void parseICMP(u_char *data)
{
    icmp_header *hdr;
    hdr = (icmp_header *)data;

    playByte(data + 0);	// type and code

    printf("[ICMP]TYPE -- 0x%x || CODE -- 0x%x\n", ntohs(hdr->type), ntohs(hdr->code));

}
