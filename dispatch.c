#include "dispatch.h"

// SKIP IF NOT IMPLEMENT
// return a pointer point to uplayer bytes
// and set type to appropriate protocol
// link type use utils to map link
u_char *link_layer_dispatch(u_char *data, int *type, int link_type)
{
    switch (link_type)
    {
    case WLAN_80211_LINK:
        fprintf(stderr,"[x]Dispatch : Not implement wlan 802.11 \n");
        return NULL;
        break;

    case ETHERNET_LINK:
        // other linker layer type follows ether by using type in dispatch
        // example :  *types = mapType(data)
        //          implement mapTypes yourself 
        *type = ntohs(((ether_header *)data)->ether_type); 
        return (data + ETHERNET_HDR_LEN);
        break;

    default:
        fprintf(stderr,"[x]Dispatch : Not Implement Unkown protocol\n");
        return NULL;
        break;
    }
}

u_char *network_layer_dispatch(u_char *data, int *type)
{
    int hdrlen;
    switch (*type)
    {
    case IPV4_NETWORK:
        *type = ((ip_header *)data)->proto;
        hdrlen = parseIP(data);
        return (data + hdrlen);
        break;
    case IPV6_NETWORK:
        fprintf(stderr, "[x]Dispatch : Not Implement IPV6 Network protocol\n");
        return NULL;
        break;
    case LOOPBACK_NETWORK:
        fprintf(stderr, "[x]Dispatch : Not Implement LOOPBACK Network protocol\n");
        return NULL;
        break;
    case ARP_NETWORK:
        *type = NO_UPPER_LAYER;
        parseARP(data);
        return (data + ARP_HDR_LEN);
        break;
    default:
        fprintf(stderr, "[x]Dispatch : Not Implement Unkown Network protocol: 0x%x\n", *type);
        return NULL;
        break;
    }
}

u_char *transport_layer_dispatch(u_char *data, int *type)
{
    int hdrlen;
    switch (*type)
    {
    case UDP_TRANSPORT:
        *type = HAS_UPPER_LAYER;
        parseUDP(data);
        return (data + UDP_HDR_LEN);
        break;
    case TCP_TRANSPORT:
        *type = HAS_UPPER_LAYER;
        hdrlen = parseTCP(data);
        return (data + hdrlen);
        break;
    case ICMP_TRANSPORT:
        *type = NO_UPPER_LAYER;
        parseICMP(data);
        return NULL;
        break;
    default:
        fprintf(stderr, "[x]Not Implement Transport Protocol : 0x%x\n", *type);
        break;
    }
}


u_char *application_layer_dispatch(u_char *data, int *type)
{
    // TODO : Any to your application here !
    return NULL;
}