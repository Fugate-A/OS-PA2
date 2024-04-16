CC = gcc
CFLAGS = -Wall -Wextra -pthread

all: chash

chash: chash.o hashdb.o rwlocks.o
	$(CC) $(CFLAGS) -o $@ $^

chash.o: chash.c hashdb.h rwlocks.h common.h common_threads.h
	$(CC) $(CFLAGS) -c -o $@ $<

hashdb.o: hashdb.c hashdb.h rwlocks.h common.h common_threads.h
	$(CC) $(CFLAGS) -c -o $@ $<

rwlocks.o: rwlocks.c rwlocks.h common.h common_threads.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f chash chash.o hashdb.o rwlocks.o
