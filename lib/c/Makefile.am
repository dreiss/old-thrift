ACLOCAL_AMFLAGS = -I ./aclocal

pkgconfigdir = $(libdir)/pkgconfig

lib_LTLIBRARIES = libthrift_c.la
#pkgconfig_DATA = thrift_c.pc

# TODO: pulled out in to configure stuffs
GLIB_CFLAGS=`pkg-config --cflags gobject-2.0`
GLIB_LDFLAGS=`pkg-config --libs gobject-2.0`


common_cflags = -Wall -W -Ithrift_c $(GLIB_CFLAGS)
common_ldflags = -Wall -W $(GLIB_LDFLAGS)

# Define the source files for the module

libthrift_c_la_SOURCES = \
		       thrift_c/protocol/thrift_binary_protocol.c \
		       thrift_c/protocol/thrift_protocol.c \
		       thrift_c/thrift_client.c \
		       thrift_c/thrift_struct.c \
		       thrift_c/transport/thrift_framed.c \
		       thrift_c/transport/thrift_socket.c \
		       thrift_c/transport/thrift_transport.c

# Flags for the various libraries

libthrift_c_la_CFLAGS =  $(common_cflags)

include_thriftdir = $(includedir)/thrift_c
include_thrift_HEADERS = \
			 thrift_c/thrift.h \
			 thrift_c/thrift_client.h \
			 thrift_c/thrift_struct.h

include_protocoldir = $(include_thriftdir)/protocol
include_protocol_HEADERS = \
			 thrift_c/protocol/thrift_binary_protocol.h \
			 thrift_c/protocol/thrift_protocol.h

include_transportdir = $(include_thriftdir)/transport
include_transport_HEADERS = \
			 thrift_c/transport/thrift_framed.h \
			 thrift_c/transport/thrift_socket.h \
			 thrift_c/transport/thrift_transport.h

#EXTRA_DIST = \
#             README \
#             thrift-nb.pc.in \
#             thrift.pc.in \
#             thrift-z.pc.in
