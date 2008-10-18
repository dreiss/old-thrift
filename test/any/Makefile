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
target: all

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
DCFL  = -Wall -O3 -g -I./gen-cpp $(include_flags)
LFL   =  -L$(thrift_home)/lib/cpp/.libs -lthrift -lthriftnb -levent
CCFL  = -Wall -O3 -I./gen-cpp $(include_flags)
CFL   = $(CCFL) $(LFL)

all: main gen-py/anytest/ttypes.py
	LD_LIBRARY_PATH=${thrift_home}/lib/cpp/.libs ./main
	python main.py

gen-cpp/anytest_types.cpp: anytest.thrift
	$(THRIFT) --gen cpp:any $<

gen-py/anytest/ttypes.py: anytest.thrift
	$(THRIFT) --gen py $<

.cpp.o:
	g++ $(DCFL) -c -o $@ $<

main: gen-cpp/anytest_types.o main.o
	g++ $(LFL) -o main $^

clean:
	rm -f *.o gen-cpp/*.o main
