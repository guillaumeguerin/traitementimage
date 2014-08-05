ROOT=../..

CC= gcc
CFLAGS= -g -Wall -Wextra -std=c99 -I$(ROOT)/include
LDFLAGS= -std=c99 -L$(ROOT)/lib -L/usr/lib/x86_64-linux-gnu/ -lbcl -lfftw3 -lm

OBJ= color_transfer.o

all: color_transfer

color_transfer: color_transfer.o
	$(CC) color_transfer.o $(LDFLAGS) -o $@


color_transfer.o: color_transfer.c

install : all
	cp color_transfer $(ROOT)/bin/

clean :
	rm -f $(OBJ) color_transfer
	rm -f *~


