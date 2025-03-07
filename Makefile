CC=gcc
CFLAGS=-Wall -Wextra -Wwrite-strings -Wno-parentheses -Wpedantic -Warray-bounds -Wconversion  -Wstrict-prototypes -std=c17 -fPIC -g -lm

SRCS=improc.c main.c
OBJS= $(SRCS:.c=.o)

TARGET=main

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS)  -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS)  $(OBJS) -o $(TARGET)

clean:
	rm -f $(OBJS) 
