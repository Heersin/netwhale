#include <pcap.h>
#include <stdio.h>
#include <time.h>

#define GET_IF_SUCCESS 0

char *get_default_dev_name(char *errbuf);
void pkt_handler(u_char *args_from_loop, const struct pcap_pkthdr *header, const u_char *pkt_data);
void parseIP(u_char *pkt_data);

/* 4字节的IP地址 */
typedef struct ip_address{
    u_char byte1;
    u_char byte2;
    u_char byte3;
    u_char byte4;
}ip_address;

/* IPv4 首部 */
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
}ip_header;

/* UDP 首部*/
typedef struct udp_header{
    u_short sport;          // 源端口(Source port)
    u_short dport;          // 目的端口(Destination port)
    u_short len;            // UDP数据包长度(Datagram length)
    u_short crc;            // 校验和(Checksum)
}udp_header;


char *get_default_dev_name(char *errbuf)
{
    char *result;
    pcap_if_t *alldevsp;

    result = NULL;

    if (GET_IF_SUCCESS == pcap_findalldevs(&alldevsp, errbuf))
    {
        if (alldevsp == NULL)
        {perror("[x]no available devs found");return -1;}

        pcap_if_t *cur_dev_p = alldevsp; 
        char *desc;
        
        if (cur_dev_p->description)
            desc = cur_dev_p->description;
        else
            desc = "None";
            
        result = cur_dev_p->name;
        printf("[*]Default Device %s : %s\n", result, desc);
    }
    else
    {
        printf("[x]Get Interface Error : %s\n", errbuf);
        exit(-1);
    }
    return result;
}

void parseIP(u_char *pkt_data)
{
    ip_header *ih;
    udp_header *uh;
    u_int ip_len;
    u_short sport,dport;
    /* 获得IP数据包头部的位置 */
    ih = (ip_header *) (pkt_data +
        14); //以太网头部长度

    /* 获得UDP首部的位置 */
    ip_len = (ih->ver_ihl & 0xf) * 4;
    uh = (udp_header *) ((u_char*)ih + ip_len);

    /* 将网络字节序列转换成主机字节序列 */
    sport = ntohs( uh->sport );
    dport = ntohs( uh->dport );

    /* 打印IP地址和UDP端口 */
    printf("%d.%d.%d.%d.%d -> %d.%d.%d.%d.%d\n",
        ih->saddr.byte1,
        ih->saddr.byte2,
        ih->saddr.byte3,
        ih->saddr.byte4,
        sport,
        ih->daddr.byte1,
        ih->daddr.byte2,
        ih->daddr.byte3,
        ih->daddr.byte4,
        dport);
}

void pkt_handler(u_char *args_from_loop, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
    struct tm ltime;
    char timestr[16];
    time_t local_tv_sec;

    local_tv_sec = header->ts.tv_sec;
    memcpy(&ltime, localtime(&local_tv_sec), sizeof(struct tm));
    strftime(timestr, sizeof(timestr), "%H:%M:%s", &ltime);
    
    printf("============ %d TH =============\n", *args_from_loop);
    *args_from_loop += 1;
    printf("[-]Time Stamp : ", timestr);
    printf("[-]Actual Capture Length : %d\n", header->caplen);
    printf("[-]Len should be  : %d\n", header->len);

    parseIP(pkt_data);
}

int main(int argc, char *argv)
{
    // Pcap Info
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // filter
    struct bpf_program fp;
    char filter_exp[] = "ip and udp";

    // Network Info
    char *dev;
    bpf_u_int32 mask;
    bpf_u_int32 net;

    // packet setting
    struct pcap_pkthdr header;
    const u_char *packet;

    /* Define the device */
    dev = get_default_dev_name(&errbuf);

    /* Get Interface infomation */
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1){
        fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
        net = 0;
        mask = 0;
    }

    /* open session for snifferning */
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if(handle == NULL){
        fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return (2);
    }
    
    /* Compile filter */
    if(pcap_compile(handle, &fp, filter_exp, 0, net) == -1){
        fprintf(stderr, "Couldn't Parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return (2);
    }
    if(pcap_setfilter(handle, &fp) == -1){
        fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return (2);
    }

    /* Grab a packet */
    u_char cnt;
    cnt = 0;
    pcap_loop(handle, 0, pkt_handler, &cnt);

    pcap_close(handle);
    return(0);
}