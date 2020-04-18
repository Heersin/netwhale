#ifndef _PKTHEADER_H
#define _PKTHEADER_H
#include <stdlib.h>
#include <stdint.h>

/*----------------------------------------------*/
/*                Link Layer                    */
/*----------------------------------------------*/
#define ETH_ALEN 6
#define ETHERNET_HDR_LEN 14

typedef struct ether_header
{
  u_char  ether_dhost[ETH_ALEN];        /* destination eth addr        */
  u_char  ether_shost[ETH_ALEN];        /* source ether addr        */
  u_short ether_type;                        /* packet type ID field        */
} __attribute__ ((__packed__)) ether_header;


/*----------------------------------------------*/
/*                  Network Layer              */
/*----------------------------------------------*/
#define ARP_HDR_LEN     8
#define IPV4_HDR_STATIC_LEN 20

// 4字节的IP地址
typedef struct ip_address{
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
}ip_address;

// IPv4 首部
typedef struct ip_header{
    u_char  ver_ihl;        // 版本 (4 bits) + 首部长度 (4 bits)
    u_char  tos;            // 服务类型(Type of service) 
    u_short tlen;           // 总长(Total length) 
    u_short identification; // 标识(Identification)
    u_short flags_fo;       // 标志位(Flags) (3 bits) + 段偏移量(Fragment offset) (13 bits)
    u_char  ttl;            // 存活时间(Time to live)
    u_char  proto;          // 协议(Protocol)
    u_short crc;            // 首部校验和(Header checksum)
    ip_address  saddr;      // 源地址(Source address)
    ip_address  daddr;      // 目的地址(Destination address)
    u_int   op_pad;         // 选项与填充(Option + Padding)
}__attribute__((packed)) ip_header;


typedef struct arp_hdr {
	uint16_t	ar_hrd;	/* format of hardware address */
	uint16_t	ar_pro;	/* format of protocol address */
	uint8_t		ar_hln;	/* length of hardware address (ETH_ADDR_LEN) */
	uint8_t		ar_pln;	/* length of protocol address (IP_ADDR_LEN) */
	uint16_t	ar_op;	/* operation */
}__attribute__((packed)) arp_header;


/*----------------------------------------------*/
/*                  Transport Layer             */
/*----------------------------------------------*/
// UDP
#define UDP_HDR_LEN 8
#define TCP_HDR_STATIC_LEN 20
#define ICMP_HDR_LEN 8

typedef struct udp_header{
    u_short sport;          // 源端口(Source port)
    u_short dport;          // 目的端口(Destination port)
    u_short len;            // UDP数据包长度(Datagram length)
    u_short crc;            // 校验和(Checksum)
}__attribute__((packed)) udp_header;

// TCP
typedef struct tcphdr
  {
    u_int16_t source;
    u_int16_t dest;
    u_int32_t seq;
    u_int32_t ack_seq;
    // little endian to parse flags
    u_int16_t res1:4;
    u_int16_t doff:4;
    u_int16_t fin:1;
    u_int16_t syn:1;
    u_int16_t rst:1;
    u_int16_t psh:1;
    u_int16_t ack:1;
    u_int16_t urg:1;
    u_int16_t res2:2;
    // end

    u_int16_t window;
    u_int16_t check;
    u_int16_t urg_ptr;
}__attribute__((packed)) tcp_header;


// ICMP
typedef struct icmphdr
{
  u_int8_t type;		/* message type */
  u_int8_t code;		/* type sub-code */
  u_int16_t checksum;
  union
  {
    struct
    {
      u_int16_t	id;
      u_int16_t	sequence;
    } echo;			/* echo datagram */
    u_int32_t	gateway;	/* gateway address */
    struct
    {
      u_int16_t	__unused;
      u_int16_t	mtu;
    } frag;			/* path mtu discovery */
  } un;
}__attribute__((packed)) icmp_header;

#endif
