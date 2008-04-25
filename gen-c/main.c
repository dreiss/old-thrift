#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#include "thrift_socket.h"
#include "thrift_protocol.h"

#define HOST "localhost"
#define PORT 2008

int main (int argc, char **argv)
{
    char hostname[100] = HOST;
    int	sd;
    struct sockaddr_in sin;
    struct sockaddr_in pin;
    struct hostent *hp;

    g_type_init ();

    ThriftSocket * socket = g_object_new (THRIFT_SOCKET_TYPE,
                                          "hostname", HOST,
                                          "port", PORT);

    // TODO: figure out why sending it as a property seg-faults
    ThriftProtocol * prot = g_object_new (THRIFT_PROTOCOL_TYPE, NULL);
    prot->thrift_socket = socket;

    if (0)
    {
        GValue host_value = {G_TYPE_STRING,};
        g_object_get_property (G_OBJECT (socket), "hostname", &host_value);

        GValue port_value = {G_TYPE_UINT,};
        g_object_get_property (G_OBJECT (socket), "port", &port_value);

        fprintf (stderr, "socket: hostname=%s, port=%d\n", 
                 g_value_get_string (&host_value),
                 g_value_get_uint (&port_value));
    }

    thrift_socket_connect (socket);

    gchar str[1024];
    g_sprintf (str, "%s\n", argv[1]);
    thrift_protocol_write_string (prot, str);

    gchar * in_buf;
    thrift_protocol_read_string (prot, &in_buf);

    /* spew-out the results and bail out of here! */
    printf ("ret: %s", in_buf);

    close (sd);
}
