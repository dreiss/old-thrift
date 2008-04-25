#!/usr/local/bin/perl
# file: tcp_echo_serv1.pl
# usage: tcp_echo_serv1.pl [port]
# Echo server, TCP version

use strict;
use Socket;
use IO::Handle;
use constant MY_ECHO_PORT => shift @ARGV || 2008;

my ($bytes_out,$bytes_in) = (0,0);

my $port = shift || MY_ECHO_PORT;
my $protocol = getprotobyname('tcp');

$SIG{'INT'} = sub {
    print STDERR "bytes_sent = $bytes_out, bytes_received = $bytes_in\n";
    exit 0;
};

socket(SOCK, AF_INET, SOCK_STREAM, $protocol) or
    die "socket() failed: $!";
setsockopt(SOCK,SOL_SOCKET,SO_REUSEADDR,1) or
    die "Can't set SO_REUSEADDR: $!";

my $my_addr = sockaddr_in($port,INADDR_ANY);
bind(SOCK,$my_addr) or
    die "bind() failed: $!";
listen(SOCK,SOMAXCONN) or
    die "listen() failed: $!";

warn "waiting for incoming connections on port $port...\n";

while(1) {
    next unless my $remote_addr = accept(SESSION,SOCK);
    my ($port,$hisaddr) = sockaddr_in($remote_addr);
    warn "Connection from [",inet_ntoa($hisaddr),",$port]\n";

    SESSION->autoflush(1);
    while(<SESSION>) {
	$bytes_in += length($_);
	chomp;

	my $msg_out = "$_\n\0";
	print STDERR $msg_out;
	print SESSION $msg_out;
	$bytes_out += length($msg_out);
    }

    warn "Connection from [",inet_ntoa($hisaddr),",$port] finished\n";
    close SESSION;
}
