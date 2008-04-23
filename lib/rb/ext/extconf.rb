require 'mkmf'

$CFLAGS = "-g -O2 -Wall -Werror"
create_makefile 'tfastbinaryprotocol'
