#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#include "protocol/thrift_binary_protocol.h"
#include "thrift.h"
#include "thrudoc.h"
#include "transport/thrift_framed.h"
#include "transport/thrift_socket.h"

#define HOST "localhost"
#define PORT 9091

int main (int argc, char **argv)
{
    THRIFT_UNUSED_VAR (argc);
    THRIFT_UNUSED_VAR (argv);

    g_type_init ();

    ThriftSocket * socket = g_object_new (THRIFT_TYPE_SOCKET,
                                          "hostname", HOST,
                                          "port", PORT, 
                                          NULL);

    ThriftFramed * framed = g_object_new (THRIFT_TYPE_FRAMED,
                                          "transport", socket,
                                          NULL);

    // TODO: figure out why sending it as a property seg-faults
    ThriftBinaryProtocol * protocol = g_object_new (THRIFT_TYPE_BINARY_PROTOCOL, 
                                                    "transport", framed, 
                                                    NULL);

    ThriftThrudocClient * client = g_object_new (THRIFT_THRUDOC_TYPE_CLIENT,
                                                 "protocol", protocol, 
                                                 NULL);

    GError * err = NULL;
    if (!thrift_transport_open (THRIFT_TRANSPORT (socket), &err))
    {
        fprintf (stderr, "failed to connect to host");
        exit (-1);
    }

    if (0)
    {
        unsigned int i;
        GPtrArray * _return;
        GError * error;
        if (thrift_thrudoc_get_buckets (client, &_return, &error))
        {
            fprintf (stderr, "len: %d\n", _return->len);
            for (i = 0; i < _return->len; i++)
            {
                fprintf (stderr, "bucket(%d): %s\n", i, 
                         (char *)g_ptr_array_index (_return, i));
                g_free (g_ptr_array_index (_return, i));
            }
            g_ptr_array_free (_return, 1);
        }
        else
        {
            fprintf (stderr, "get_buckets error: %s\n", 
                     error ? error->message : "unknown");

        }
    }
    if (0)
    {
        gchar * _return;
        thrift_thrudoc_admin (client, &_return, "echo", "data", NULL);
        fprintf (stderr, "admin ('echo', 'data')=%s\n", _return);
        g_free (_return);
    }
    if (0)
    {
        gchar * _return;
        thrift_thrudoc_put (client, "bucket", "key", "value", NULL);
        thrift_thrudoc_get (client, &_return, "bucket", "key", NULL);
        fprintf (stderr, "put/get ('bucket', 'key', 'value)=%s\n", _return);
        g_free (_return);
    }
    if (1)
    {
        guint i;
        GPtrArray * _return;
        GError * error = NULL;
        GPtrArray * elements = g_ptr_array_new ();
        ThriftThrudocElement * element = 
          g_object_new (THRIFT_THRUDOC_TYPE_ELEMENT, NULL);
        element->bucket = "bucket";
        element->key = "key1";
        element->value = "value1";
        g_ptr_array_add (elements, element);
        element = g_object_new (THRIFT_THRUDOC_TYPE_ELEMENT, NULL);
        element->bucket = "bucket";
        element->key = "key2";
        element->value = "value2";
        g_ptr_array_add (elements, element);
        if (!thrift_thrudoc_put_list (client, &_return, elements, &error))
        {
          fprintf (stderr, "put_list error: %s", 
                   error ? error->message : "unknown");
        }
        for (i = 0; i < _return->len; i++)
        {
            GError * ex = g_ptr_array_index (_return, i);
            fprintf (stderr, "put_list(%d) ex: %s\n", i, 
                     ex ? ex->message : "none");
            if (ex != NULL) g_error_free (ex);
        }
        g_ptr_array_free (_return, 1);

        if (!thrift_thrudoc_get_list (client, &_return, elements, &error))
        {
          fprintf (stderr, "put_list error: %s", 
                   error ? error->message : "unknown");
        }
        for (i = 0; i < _return->len; i++)
        {
            ThriftThrudocListResponse * resp = g_ptr_array_index (_return, i);
            fprintf (stderr, "get_list(%d) element: (%s, %s, %s), ex: %s\n", i, 
                     resp->element->bucket, resp->element->key, 
                     resp->element->value, 
                     resp->ex ? resp->ex->message : "none");
            if (resp != NULL) g_object_unref (resp);
        }
        g_ptr_array_free (_return, 1);

        for (i = 0; i < elements->len; i++)
            g_object_unref (g_ptr_array_index (elements, i));
        g_ptr_array_free (elements, 1);
    }

    g_object_unref (socket);
    g_object_unref (framed);
    g_object_unref (protocol);
    g_object_unref (client);

    return 0;
}
