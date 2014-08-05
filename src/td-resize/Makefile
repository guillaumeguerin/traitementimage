ROOT=../..

CC= gcc
CFLAGS= -g -Wall -Wextra -std=c99 -I$(ROOT)/include
LDFLAGS= -std=c99 -L$(ROOT)/lib -L/usr/lib/x86_64-linux-gnu/ -lbcl -lfftw3 -lm

OBJ= fft.o resize_copy.o zero_padding.o resize_filter.o

all: resize_copy zero_padding resize_filter

resize_copy: resize_copy.o fft.o
	$(CC) resize_copy.o fft.o $(LDFLAGS) -o $@

zero_padding: zero_padding.o fft.o
	$(CC) zero_padding.o fft.o $(LDFLAGS) -o $@

resize_filter: resize_filter.o fft.o
	$(CC) resize_filter.o fft.o $(LDFLAGS) -o $@

fft.o: fft.c fft.h
resize_copy.o: resize_copy.c 
resize_filter.o: resize_filter.c
zero_padding.o: zero_padding.c fft.h fft.c

install : all
	cp resize_copy zero_padding resize_filter $(ROOT)/bin/

clean :
	rm -f $(OBJ) resize_copy zero_padding resize_filter
	rm -f *~


