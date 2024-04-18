CC = gcc
CFLAGS = -Wall -Wextra -pthread

all: chash

chash: chash.o hashdb.o rwlocks.o
	$(CC) $(CFLAGS) -o chash chash.o hashdb.o rwlocks.o

chash.o: chash.c hashdb.h rwlocks.h
	$(CC) $(CFLAGS) -c chash.c

hashdb.o: hashdb.c hashdb.h rwlocks.h
	$(CC) $(CFLAGS) -c hashdb.c

rwlocks.o: rwlocks.c rwlocks.h common_threads.h
	$(CC) $(CFLAGS) -c rwlocks.c

clean:
	rm -f *.o chash
