CC = gcc
CFLAGS = -w -g

all: calServer calClient

calServer: calServer.o
	$(CC) $(CFLAGS) -o calServer calServer.o

calClient: calClient.o cleanver.o
	$(CC) $(CFLAGS) -o calClient calClient.o cleanver.o -lncurses

calServer.o: calServer.c
	$(CC) $(CFLAGS) -c calServer.c

calClient.o: calClient.c calClient.h
	$(CC) $(CFLAGS) -c calClient.c

cleanver.o: cleanver.c calClient.h
	$(CC) $(CFLAGS) -c cleanver.c

clean:
	rm -f calServer calClient *.o

