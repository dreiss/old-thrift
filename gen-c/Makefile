FILES = thrudoc_types.c thrudoc.c main.c
CFLAGS = `pkg-config --cflags gobject-2.0` -I$(SANDBOX)/include/thrift_c -g -Wall -W
LDFLAGS = `pkg-config --libs gobject-2.0` -L$(SANDBOX)/lib -lthrift_c

all: test

test: Makefile $(FILES)
	gcc -o $@ $(CFLAGS) $(LDFLAGS) $(FILES)
