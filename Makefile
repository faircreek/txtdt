OBJS=parse.o txtdt.o

CPPFLAGS=-I.
CFLAGS=-g
CC=gcc

all: $(OBJS)
	$(CC) -o txtdt $(OBJS) 


