#ifndef _PARSE_H
#define _PARSE_H
#include <pcap.h>
#include "pkthdr.h"
#include "utils.h"
// if header length is not fixed, then return the real length
int parseIP(u_char *pkt_data);
void parseARP(u_char *pkt_data);
void parseUDP(u_char *data);
int parseTCP(u_char *data);
void parseICMP(u_char *data);

#endif