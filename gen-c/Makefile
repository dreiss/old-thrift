FILES = thrift_socket.c thrift_transport.c thrift_protocol.c thrift_binary_protocol.c thrift_client.c thrift_struct.c thrudoc_types.c thrudoc.c main.c
CFLAGS = `pkg-config --cflags gobject-2.0` -g # -Wall -W
LDFLAGS = `pkg-config --libs gobject-2.0`

all: test

test: Makefile $(FILES)
	gcc -o $@ $(CFLAGS) $(LDFLAGS) $(FILES)
