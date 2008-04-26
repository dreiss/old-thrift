FILES = thrudoc.c thrudoc_types.c thrift_struct.c thrift_socket.c thrift_protocol.c main.c
CFLAGS = `pkg-config --cflags gobject-2.0` -g # -Wall -W
LDFLAGS = `pkg-config --libs gobject-2.0`

all: test

test: Makefile $(FILES)
	gcc -o $@ $(CFLAGS) $(LDFLAGS) $(FILES)
