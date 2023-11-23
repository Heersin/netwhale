#include <pcap.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "parse.h"
#include "dispatch.h"
#include "utils.h"
#define BPF_FILTER_SIZE 1024
#define NO_FILTER 0
#define HAS_FILTER 1

// GLOBAL DEVICE
ao_device *main_device;

void pkt_handler(u_char *args_from_loop, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
    int type = -1;
    u_char *data;

    data = link_layer_dispatch((u_char *) pkt_data, &type, ETHERNET_LINK);
    if (type == NO_UPPER_LAYER)
    {
        fprintf(stderr, "[DEBUG] Link: No Upper Layer\n");
        return;
    }
    if (data == NULL)
    {
        fprintf(stderr, "[DEBUG] Link: Skip\n");
        return;
    }

    data = network_layer_dispatch(data, &type);
    if (type == NO_UPPER_LAYER)
    {
        fprintf(stderr, "[DEBUG] Network : No Upper Layer\n");
        return;
    }
    if (data == NULL)
    {
        fprintf(stderr, "[DEBUG] Network : Skip\n");
        return;
    }

    data = transport_layer_dispatch(data, &type);
    if (type == NO_UPPER_LAYER)
    {
        fprintf(stderr, "[DEBUG] Transport : No Upper Layer\n");
        return;
    }
    if (data == NULL)
    {
        fprintf(stderr, "[DEBUG] Transport : Skip\n");
        return;
    }
}

int main(int argc, char *argv[])
{
    // Cmd Args
    int ch;
    // Pcap Info
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    // filter
    struct bpf_program fp;
    char filter_exp[BPF_FILTER_SIZE] = {0};
    int has_filter = NO_FILTER;
    // Network Info
    char *dev;
    int link_layer_type_code;
    bpf_u_int32 mask;
    bpf_u_int32 net;
    // packet setting
    struct pcap_pkthdr header;
    const u_char *packet;

    /*--------------- CMD ISSUE ----------------*/
    /*------------------------------------------*/
    // init some args
    dev = NULL;
    link_layer_type_code = ETHERNET_LINK;
    while ((ch = getopt(argc, argv, "f:t:i:h")) != -1)
    {
        switch (ch)
        {
        case 'i':
            printf("Manual Chose NetCard : '%s'\n", (char *) optarg);
            printf("Warning : Your should set interface link type by '-t'");
            printf("    Support Type in Help\n");
            dev = (char *) optarg;
            break;

        case 't':
            link_layer_type_code = atoi(optarg);
            printf("Manual Set Interface Type : 0x%x\n", link_layer_type_code);
            break;

        case 'f':
            strncpy(filter_exp, optarg, BPF_FILTER_SIZE);
            has_filter = HAS_FILTER;
            break;

        case 'l':
            printf("sorry , this will imeplment in next version\n");
            break;

        case 'h':
            // no break; fall through
        default:
            printf("help");
            return 0;
        }
    }

    /* -------------- Net Filter ISSUE -----------*/
    /* -------------------------------------------*/
    /* Define the device */
    if (NULL == dev)
    {
        dev = get_default_dev_name(errbuf);
    }

    /* Get Interface infomation */
    if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1)
    {
        fprintf(stderr, "[-]Couldn't get netmask for device %s: %s\n", dev, errbuf);
        net = 0;
        mask = 0;
    }

    /* open session for snifferning */
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL)
    {
        fprintf(stderr, "[-]Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
        return (2);
    }

    /* Compile filter */
    if (has_filter == HAS_FILTER)
    {
        if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1)
        {
            fprintf(stderr, "[-]Couldn't Parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
            return (2);
        }
        if (pcap_setfilter(handle, &fp) == -1)
        {
            fprintf(stderr, "[-]Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
            return (2);
        }
    }

    // Aduio Handle
    ao_sample_format format;
    int default_driver;
    char *buffer;
    int buf_size;

    // Create Mutex
    // pthread_mutex_init(&mutex, NULL);

    ao_initialize();
    default_driver = ao_default_driver_id();
    printf("default_driver : %d\n", default_driver);

    // Set Some Audio Args
    memset(&format, 0, sizeof(format));
    format.bits = FORMAT_BITS;
    format.channels = FORMAT_CHANNEL;
    format.rate = FORMAT_RATE;
    format.byte_format = BYTE_FORMAT;

    // open device
    main_device = ao_open_live(default_driver, &format, NULL /* no options */);
    if (main_device == NULL)
    {
        fprintf(stderr, "Error opening device.\n");
        return 1;
    }

    /* Grab a packet */
    u_char cnt;
    cnt = 0;
    pcap_loop(handle, 0, pkt_handler, &cnt);
    ao_close(main_device);
    ao_shutdown();

    // destroy mutex
    // pthread_mutex_destroy(&mutex);

    pcap_close(handle);
    return (0);
}
