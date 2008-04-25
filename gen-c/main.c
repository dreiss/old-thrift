#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#include "thrift_socket.h"
#include "thrift_protocol.h"
#include "Thrudoc_types.h"

#define HOST "localhost"
#define PORT 2008

guint32 Thrudoc_admin_args_write (ThriftProtocol * prot, const gchar * op, 
                                  const gchar * data)
{
    guint32 xfer = 0;
    xfer += thrift_protocol_write_struct_begin (prot, "Thrudoc_admin_args");
    xfer += thrift_protocol_write_field_begin (prot, "op", T_STRING, 1);
    xfer += thrift_protocol_write_string (prot, op);
    xfer += thrift_protocol_write_field_end (prot);
    xfer += thrift_protocol_write_field_begin (prot, "data", T_STRING, 2);
    xfer += thrift_protocol_write_string (prot, data);
    xfer += thrift_protocol_write_field_end (prot);
    xfer += thrift_protocol_write_field_stop (prot);
    xfer += thrift_protocol_write_struct_end (prot);
    return xfer;
}

void ThrudocClient_send_admin (ThriftProtocol * prot, const gchar * op,
                              const gchar * data)
{
  gint32 cseqid = 0;

  thrift_protocol_write_message_begin (prot, "admin", T_CALL, cseqid);

  Thrudoc_admin_args_write (prot, op, data);

  thrift_protocol_write_message_end (prot);
}

guint32 Thrudoc_putList_args_write (ThriftProtocol * prot, GPtrArray * elements)
{
    uint32_t xfer = 0;
    xfer += thrift_protocol_write_struct_begin (prot, "Thrudoc_putList_args");
    xfer += thrift_protocol_write_field_begin (prot, "elements", T_LIST, 1);
    {
        int i;
        xfer += thrift_protocol_write_list_begin (prot, T_STRUCT, elements->len);
        for (i = 0; i < elements->len; i++)
        {
            xfer += thrift_struct_write 
                (THRIFT_STRUCT (g_ptr_array_index (elements, i)), prot);
        }
        xfer += thrift_protocol_write_list_end (prot);
    }
    xfer += thrift_protocol_write_field_end (prot);
    xfer += thrift_protocol_write_field_stop (prot);
    xfer += thrift_protocol_write_struct_end (prot);
    return xfer;
}

void ThrudocClient_send_putList (ThriftProtocol * prot, GPtrArray * elements)
{
  gint32 cseqid = 0;

  thrift_protocol_write_message_begin (prot, "putList", T_CALL, cseqid);

  Thrudoc_putList_args_write (prot, elements);

  thrift_protocol_write_message_end (prot);
}

int main (int argc, char **argv)
{
    char hostname[100] = HOST;
    int	sd;
    struct sockaddr_in sin;
    struct sockaddr_in pin;
    struct hostent *hp;

    g_type_init ();

    ThriftSocket * socket = g_object_new (THRIFT_TYPE_SOCKET,
                                          "hostname", HOST,
                                          "port", PORT, 
                                          NULL);

    // TODO: figure out why sending it as a property seg-faults
    ThriftProtocol * prot = g_object_new (THRIFT_TYPE_PROTOCOL, 
                                          "socket", socket, 
                                          NULL);

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

    ThrudocClient_send_admin (prot, "echo", "hello there");

    GPtrArray * elements = g_ptr_array_new ();
    ThriftThrudocElement * te = g_object_new (THRIFT_THRUDOC_TYPE_ELEMENT, 
                                              NULL);
    te->bucket = "bucket1";
    te->key = "key1";
    te->value = "value1";
    g_ptr_array_add (elements, te);
    te = g_object_new (THRIFT_THRUDOC_TYPE_ELEMENT, NULL);
    te->bucket = "bucket2";
    te->key = "key2";
    te->value = "value2";
    g_ptr_array_add (elements, te);
    ThrudocClient_send_putList (prot, elements);
    g_ptr_array_free (elements, 1);

    close (sd);
}
