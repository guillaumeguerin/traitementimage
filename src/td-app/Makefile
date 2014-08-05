ROOT=../..

CC= gcc
CFLAGS= -g -Wall -Wextra -std=c99 -I$(ROOT)/include
LDFLAGS= -std=c99 -L$(ROOT)/lib -lbcl -lfftw3

OBJ= rotate.o

all: rotate

rotate: rotate.o
	$(CC) rotate.o $(LDFLAGS) -o $@

rotate.o: rotate.c

install : all
	cp rotate $(ROOT)/bin/

clean :
	rm -f $(OBJ) rotate
	rm -f *~


