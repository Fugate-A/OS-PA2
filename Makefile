CC = gcc
CFLAGS = -Wall -Wextra -pthread
SRCS = chash.c hashdb.c rwlocks.c
OBJS = $(SRCS:.c=.o)
TARGET = chash

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
