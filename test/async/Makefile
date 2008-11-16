# Makefile for Thrift test project.
#
# Author:
#   Mark Slee <mcslee@facebook.com>

# Default target is everything

ifndef thrift_home
thrift_home=../..
endif #thrift_home

target: all

ifndef boost_home
#boost_home=../../../../../thirdparty/boost_1_33_1
boost_home=/usr/local/include/boost-1_33_1
endif #boost_home

include_paths = $(thrift_home)/lib/cpp/src \
		$(boost_home)

include_flags = $(patsubst %,-I%, $(include_paths))

# Tools
ifndef THRIFT
THRIFT = ../../compiler/cpp/thrift
endif # THRIFT

CC     = g++
LD     = g++

# Compiler flags
DCFL  = -Wall -O3 -g -I./gen-cpp $(include_flags) -L$(thrift_home)/lib/cpp/.libs -lthrift -lthriftnb -levent
LFL   =  -L$(thrift_home)/lib/cpp/.libs -lthrift -lthriftnb -levent
CCFL  = -Wall -O3 -I./gen-cpp $(include_flags)
CFL   = $(CCFL) $(LFL)

all: server client

stubs: ../ThriftTest.thrift
	$(THRIFT) --gen cpp ../ThriftTest.thrift

.cpp.o:
	$(CC) -c -o $@ $(CCFL) $<

gen-cpp/ThriftTest.o: stubs
gen-cpp/ThriftTest_types.o: stubs

server: ./gen-cpp/ThriftTest.o ./gen-cpp/ThriftTest_types.o src/TestServer.o
	$(CC) -o TestServer $(CFL) $^

client: ./gen-cpp/ThriftTest.o ./gen-cpp/ThriftTest_types.o src/TestClient.o
	$(CC) -o TestClient $(CFL) $^

clean:
	rm -fr *.o src/*.o TestServer TestClient gen-cpp
