netwhale: main.o utils.o dispatch.o parse.o
	gcc -o netwhale main.o utils.o parse.o dispatch.o -lpcap

main.o: main.c parse.h utils.h
	gcc -c -g main.c -lpcap

utils.o: utils.c utils.h
	gcc -c -g utils.c -lpcap

dispatch.o: dispatch.c dispatch.h parse.h pkthdr.h parse.o
	gcc -c -g dispatch.c -lpcap

parse.o: parse.c utils.h parse.h pkthdr.h
	gcc -c -g parse.c -lpcap

clean:
	rm -f *.o