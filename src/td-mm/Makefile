ROOT=../..

CC= gcc
CFLAGS= -g -Wall -Wextra -std=c99 -I$(ROOT)/include
LDFLAGS= -std=c99 -L$(ROOT)/lib -L/usr/lib/x86_64-linux-gnu/ -lbcl -lfftw3 -lm

OBJ= mm.o mm_dil.o mm_ero.o mm_open.o mm_close.o mm_substract.o mm_add.o mm_binarize.o mm_label.o

all: mm_dil mm_ero mm_open mm_close mm_substract mm_add mm_binarize mm_label

mm_dil: mm_dil.o mm.o
	$(CC) mm_dil.o mm.o $(LDFLAGS) -o $@

mm_ero: mm_ero.o mm.o
	$(CC) mm_ero.o mm.o $(LDFLAGS) -o $@

mm_open: mm_open.o mm.o
	$(CC) mm_open.o mm.o $(LDFLAGS) -o $@

mm_close: mm_close.o mm.o
	$(CC) mm_close.o mm.o $(LDFLAGS) -o $@

mm_substract: mm_substract.o
	$(CC) mm_substract.o $(LDFLAGS) -o $@

mm_add: mm_add.o
	$(CC) mm_add.o $(LDFLAGS) -o $@

mm_binarize: mm_binarize.o
	$(CC) mm_binarize.o $(LDFLAGS) -o $@

mm_label: mm_label.o
	$(CC) mm_label.o $(LDFLAGS) -o $@

mm.o: mm.c mm.h
mm_dil.o: mm_dil.c
mm_ero.o: mm_ero.c
mm_open.o: mm_open.c
mm_close.o: mm_close.c
mm_substract.o: mm_substract.c
mm_add.o: mm_add.c
mm_binarize.o: mm_binarize.c
mm_label.o: mm_label.c

install : all
	cp mm_dil mm_ero mm_open mm_close mm_substract mm_add mm_binarize mm_label $(ROOT)/bin/

clean :
	rm -f $(OBJ) mm_dil mm_ero mm_open mm_close mm_substract mm_add mm_binarize mm_label
	rm -f *~


