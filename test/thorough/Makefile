
all: case thorough thorough_server

case_FILES = gen-c/case_service.c gen-c/case_types.c gen-c/s_e_r_v_i_c_e_a_l_l_c_a_p_s.c gen-c/service_initial_caps.c gen-c/service__initial__caps__with__underscores.c gen-c/servicelowercase.c gen-c/service_lowercase_with_underscores.c
case_CFLAGS = `pkg-config --cflags gobject-2.0` -I$(SANDBOX)/include/thrift_c -g -Wall -W
case_LDFLAGS = `pkg-config --libs gobject-2.0` -L$(SANDBOX)/lib -lthrift_c

case: main.c $(case_FILES) 
	gcc -o $@ $(case_CFLAGS) $(case_LDFLAGS) $+

$(case_FILES) : case.thrift
	thrift --gen c $<

thorough_FILES = gen-c/thorough_types.c gen-c/thorough.c
thorough_CFLAGS = `pkg-config --cflags gobject-2.0` -I$(SANDBOX)/include/thrift_c -g -Wall -W
thorough_LDFLAGS = `pkg-config --libs gobject-2.0` -L$(SANDBOX)/lib -lthrift_c

thorough: thorough.c $(thorough_FILES) 
	gcc -o $@ $(thorough_CFLAGS) $(thorough_LDFLAGS) $+

$(thorough_FILES) : thorough.thrift
	thrift --gen c $<


thorough_server_FILES = gen-cpp/thorough_constants.cpp gen-cpp/Thorough.cpp gen-cpp/thorough_types.cpp
thorough_server_CFLAGS = `pkg-config --cflags thrift-nb` -g -Wall
thorough_server_LDFLAGS = `pkg-config --libs thrift-nb` -g -levent

thorough_server: Thorough_server.cpp $(thorough_server_FILES) 
	gcc -o $@ $(thorough_server_CFLAGS) $(thorough_server_LDFLAGS) $+

$(thorough_server_FILES) : thorough.thrift
	thrift --gen cpp $<


