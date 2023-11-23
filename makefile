CFLAGS+= -g -O2 `pkg-config --cflags --libs ao`

all: netwhale

netwhale: main.o utils.o dispatch.o parse.o
	gcc -o netwhale main.o utils.o parse.o dispatch.o -lpcap -lm $(CFLAGS)

main.o: main.c parse.h utils.h
	gcc -c -g main.c -lpcap $(CFLAGS)

utils.o: utils.c utils.h
	gcc -c -g utils.c -lpcap $(CFLAGS)

dispatch.o: dispatch.c dispatch.h parse.h pkthdr.h parse.o
	gcc -c -g dispatch.c -lpcap $(CFLAGS)

parse.o: parse.c utils.h parse.h pkthdr.h
	gcc -c -g parse.c -lpcap $(CFLAGS)

clean:
	rm -f netwhale *.o
