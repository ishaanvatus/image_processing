CC = gcc
CFLAGS = -Wall -Wpedantic -Wextra
OFLAGS = -O3
DFLAGS = -g

SRCS = improc.c main.c
OBJS = $(SRCS:.c=.o)

TARGET = main

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(OFLAGS) $(DFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OFLAGS) $(DFLAGS) $(OBJS) -o $(TARGET)

clean:
	rm -f $(OBJS) 
