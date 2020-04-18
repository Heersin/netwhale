#ifndef _UTILS_H
#define _UTILS_H
// Network Related
#include <pcap.h>
#define GET_IF_SUCCESS 0
char *get_default_dev_name(char *errbuf);

// Audio Related
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ao/ao.h>
#include <math.h>
#include <unistd.h>
extern ao_device *main_device;
extern pthread_mutex_t mutex;

#define BUF_SIZE 4096

#define C 261
#define CP 277
#define D 294
#define DP 311
#define E 329
#define F 349
#define FP 369
#define G 392
#define GP 415
#define A 440
#define AP 466
#define B 494

#define C_ 523
#define C_P 554
#define D_ 587
#define D_P 622


#define FORMAT_BITS 16
#define FORMAT_CHANNEL 2
#define FORMAT_RATE 44100
#define BYTE_FORMAT AO_FMT_LITTLE

void playTone(int freq, ao_device *device, float keep_time);
int mapTone(u_char byte);
void playByte(u_char byte);
void help();
#endif
