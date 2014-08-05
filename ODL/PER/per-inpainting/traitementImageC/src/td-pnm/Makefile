ROOT=../..

CC=gcc
CFLAGS= -g -Wall -Wextra -std=c99 -I$(ROOT)/include
LDFLAGS= -std=c99 -I$(ROOT)/include -L$(ROOT)/lib -lbcl 

%OBJ= test1.o 

all: test1 extract_subimage extract_channel gray2color color2mean normalize

test1: test1.o 
	$(CC)  test1.o $(LDFLAGS) -o test1

test1.o: test1.c 

print_ppm: print_ppm.o
	$(CC)  print_ppm.o $(LDFLAGS) -o print_ppm

print_ppm.o: print_ppm.c

extract_subimage: extract_subimage.o
	$(CC)  extract_subimage.o $(LDFLAGS) -o extract_subimage

extract_subimage.o: extract_subimage.c

extract_channel: extract_channel.o
	$(CC)  extract_channel.o $(LDFLAGS) -o extract_channel

extract_channel.o: extract_channel.c

gray2color: gray2color.o
	$(CC)  gray2color.o $(LDFLAGS) -o gray2color

gray2color.o: gray2color.c 

color2mean: color2mean.o
	$(CC)  color2mean.o $(LDFLAGS) -o color2mean

color2mean.o: color2mean.c

normalize: normalize.o
	$(CC)  normalize.o $(LDFLAGS) -o normalize

normalize.o: normalize.c

install : all
	cp test1 $(ROOT)/bin/

clean :
	rm -f $(OBJ) test1 extract_subimage extract_channel grey2color color2mean normalize


