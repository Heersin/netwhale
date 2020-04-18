#include "utils.h"

void help()
{
    printf("Netwhale handle your packets and proc to audio, listen to network ~\n");
    printf("[Usage] sudo ./netwhale\n");
    printf("[More Usage] : \n");
    printf("-f [bpf statement] : Use Filter in packet capturing, it's optional, default capture all packets\n");
    printf("-i [interface name] : Use which card, you can find your network interface by using 'ifconfig', it's optional but should work with -t\n");
    printf("-t [link layer type] : If you know the type of your link layer, set this, default as Ethernet \n");
    printf("    The Type code as Follows => \n");
    printf("    [1] Ethernet -- 0x00 \n");
    printf("    [2] WLAN 802.11 -- 0x01\n");
    printf("-h : this help page\n");
    printf("-lx : contains l2, l3, l4, l5, design to filter the audio, such as '-l3' only listen to network layer, not implement\n");
}

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

// Not Thread Safety
void playTone(int freq, ao_device *device, float keep_time)
{
	char *buffer;
	int sample;
	int i;
	int buf_size;
	int play_bits;

	play_bits = (int)(FORMAT_RATE * keep_time);
	buf_size = FORMAT_BITS/8 * FORMAT_CHANNEL * play_bits;
	
	
	
	buffer = calloc(buf_size,
			sizeof(char));

	if(buffer == NULL){
		fprintf(stderr, "calloc failed");
		return;
	}

	for (i = 0; i < play_bits; i++) {
		sample = (int)(0.75 * 32768.0 *
			sin(2 * M_PI * freq * ((float) i/FORMAT_RATE)));

		/* Put the same stuff in left and right channel */
		buffer[4*i] = sample & 0xff; 
		buffer[4*i+2] = sample & 0xff;
		buffer[4*i+1] = (sample >> 8) & 0xff;
		buffer[4*i+3] = (sample >> 8) & 0xff;
	}

    ao_play(device, buffer, buf_size);
    usleep(110370);

}

int mapTone(u_char byte)
{
    // 200 is the lowest freq which can be display in my computer.
    // and 1300 freq is OK
    // so the byte data (0~255) -> map to (200 ~ 1020)
    // Normal can disguish 2 freq if their diffrence < 2 when freq under 1000hz
    // 
    int result;
    result = ((int)byte) << 2;
    result += 200;
    return result;
}


void playByte(u_char byte)
{
	playTone(mapTone(byte), main_device, 0.5 );
}
