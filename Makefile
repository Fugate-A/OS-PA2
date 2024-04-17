CC = gcc

CFLAGS = -Wall -Wextra -pthread

SRCS = chash.c hashdb.c rwlocks.c

OBJS = $(SRCS:.c=.o)

TARGET = chash

all: $(TARGET)

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
    $(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

clean:
    rm -f $(OBJS) $(TARGET)

.PHONY: all clean 