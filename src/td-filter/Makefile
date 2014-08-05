ROOT=../..

CC= gcc
CFLAGS= -g -Wall -Wextra -std=c99 -I$(ROOT)/include
LDFLAGS= -std=c99 -L$(ROOT)/lib -lbcl -lfftw3

OBJ= fft.o heat_equation.o anisotropic_diffusion.o bilateral nlmeans.o butterworth.o

all: heat_equation anisotropic_diffusion bilateral nlmeans butterworth

butterworth: butterworth.o fft.o
	$(CC) butterworth.o fft.o $(LDFLAGS) -o $@
	
heat_equation: heat_equation.o
	$(CC) heat_equation.o $(LDFLAGS) -o $@
	
anisotropic_diffusion: anisotropic_diffusion.o
	$(CC) anisotropic_diffusion.o $(LDFLAGS) -o $@
	
bilateral: bilateral.o
	$(CC) bilateral.o $(LDFLAGS) -o $@
	
nlmeans: nlmeans.o
	$(CC) nlmeans.o $(LDFLAGS) -o $@

fft.o: fft.c fft.h
butterworth.o: butterworth.c fft.h

install : all
	cp butterworth $(ROOT)/bin/

clean :
	rm -f $(OBJ) butterworth
	rm -f *~


