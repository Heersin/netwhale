/*
 *
 * ao_example.c
 *
 *     Written by Stan Seibert - July 2001
 *
 * Legal Terms:
 *
 *     This source file is released into the public domain.  It is
 *     distributed without any warranty; without even the implied
 *     warranty * of merchantability or fitness for a particular
 *     purpose.
 *
 * Function:
 *
 *     This program opens the default driver and plays a 440 Hz tone for
 *     one second.
 *
 * Compilation command line (for Linux systems):
 *
 *     gcc -o ao_example ao_example.c -lao -ldl -lm
 *
 */

#include <stdio.h>
#include <string.h>
#include <ao/ao.h>
#include <math.h>
#include <unistd.h>

#define BUF_SIZE 4096
#define A 440
#define B 494
#define C 261
#define D 294
#define E 329
#define F 349
#define G 392

void playTone(int freq, ao_device *device, float keep_time);

void playTone(int freq, ao_device *device, float keep_time)
{
	char *buffer;
	int sample;
	int i;
	int buf_size;
	int play_bits;
	ao_sample_format format;

	memset(&format, 0, sizeof(format));
	format.bits = 16;
	format.channels = 2;
	format.rate = 44100;
	format.byte_format = AO_FMT_LITTLE;

	play_bits = (int)(format.rate * keep_time);

	buf_size = format.bits / 8 * format.channels * play_bits;
	buffer = calloc(buf_size,
					sizeof(char));

	for (i = 0; i < play_bits; i++)
	{
		sample = (int)(0.75 * 32768.0 *
					   sin(2 * M_PI * freq * ((float)i / format.rate)));

		/* Put the same stuff in left and right channel */
		buffer[4 * i] = buffer[4 * i + 2] = sample & 0xff;
		buffer[4 * i + 1] = buffer[4 * i + 3] = (sample >> 8) & 0xff;
	}
	ao_play(device, buffer, buf_size);
	usleep(130000);
}

int main(int argc, char **argv)
{
	ao_device *device;
	ao_sample_format format;
	int default_driver;
	char *buffer;
	int buf_size;
	int sample;
	float freq = 423.0;
	int i;

	/* -- Initialize -- */

	fprintf(stderr, "libao example program\n");

	ao_initialize();

	/* -- Setup for default driver -- */

	default_driver = ao_default_driver_id();

	memset(&format, 0, sizeof(format));
	format.bits = 16;
	format.channels = 2;
	format.rate = 44100;
	format.byte_format = AO_FMT_LITTLE;

	/* -- Open driver -- */
	device = ao_open_live(default_driver, &format, NULL /* no options */);
	if (device == NULL)
	{
		fprintf(stderr, "Error opening device.\n");
		return 1;
	}

	playTone(C, device, 0.5);
	playTone(C, device, 0.5);
	playTone(G, device, 0.5);
	playTone(G, device, 0.5);
	playTone(A, device, 0.5);
	playTone(A, device, 0.5);
	playTone(G, device, 0.5);

	usleep(300000);

	playTone(F, device, 0.5);
	playTone(F, device, 0.5);
	playTone(E, device, 0.5);
	playTone(E, device, 0.5);
	playTone(D, device, 0.5);
	playTone(D, device, 0.5);
	playTone(C, device, 0.5);

	usleep(300000);

	playTone(G, device, 0.5);
	playTone(G, device, 0.5);
	playTone(F, device, 0.5);
	playTone(F, device, 0.5);
	playTone(E, device, 0.5);
	playTone(E, device, 0.5);
	playTone(D, device, 0.5);

	usleep(300000);

	playTone(G, device, 0.5);
	playTone(G, device, 0.5);
	playTone(F, device, 0.5);
	playTone(F, device, 0.5);
	playTone(E, device, 0.5);
	playTone(E, device, 0.5);
	playTone(D, device, 0.5);

	usleep(300000);

	playTone(C, device, 0.5);
	playTone(C, device, 0.5);
	playTone(G, device, 0.5);
	playTone(G, device, 0.5);
	playTone(A, device, 0.5);
	playTone(A, device, 0.5);
	playTone(G, device, 0.5);

	usleep(300000);

	playTone(F, device, 0.5);
	playTone(F, device, 0.5);
	playTone(E, device, 0.5);
	playTone(E, device, 0.5);
	playTone(D, device, 0.5);
	playTone(D, device, 0.5);
	playTone(C, device, 0.5);
	/* -- Close and shutdown -- */
	ao_close(device);

	ao_shutdown();

	return (0);
}
