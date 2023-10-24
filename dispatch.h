#ifndef _DISPATCH_H
#define _DISPATCH_H
#include <stdlib.h>
#include "pkthdr.h"
#include "parse.h"

#define NO_UPPER_LAYER -1
#define HAS_UPPER_LAYER 0
/*----------------------------------------------*/
/*                Link Layer                    */
/*----------------------------------------------*/
// ethernet
#define ETHERNET_HDR_LEN 14
#define ETHERNET_LINK 0x0

// wlan
#define WLAN_80211_LINK 0x1

// functions
u_char *link_layer_dispatch(u_char *data, int *type, int link_type);

/*----------------------------------------------*/
/*                  Network Layer              */
/*----------------------------------------------*/
#define IPV4_HDR_STATIC_LEN 20
#define IPV4_NETWORK 0x0800

#define IPV6_NETWORK 0x86dd

#define LOOPBACK_NETWORK 0x9000

#define ARP_HDR_LEN 8
#define ARP_NETWORK 0x0806

// functions
u_char *network_layer_dispatch(u_char *data, int *type);

/*----------------------------------------------*/
/*                  Transport Layer             */
/*----------------------------------------------*/
#define UDP_HDR_LEN 8
#define UDP_TRANSPORT 0x11

#define TCP_HDR_STATIC_LEN 20 // the length of fixed part
#define TCP_TRANSPORT 0x06

#define ICMP_HDR_LEN 8
#define ICMP_TRANSPORT 0x01

// functions
u_char *transport_layer_dispatch(u_char *data, int *type);

/*----------------------------------------------*/
/*                  Transport Layer             */
/*----------------------------------------------*/
u_char *application_layer_dispatch(u_char *data, int *type);

#endif
