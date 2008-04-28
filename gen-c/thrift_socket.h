/**
 * Thrift c
 *
 */
#ifndef THRIFT_SOCKET_H
#define THRIFT_SOCKET_H

#include "thrift_transport.h"

#include <glib-object.h>

typedef struct _ThriftSocket ThriftSocket;
struct _ThriftSocket
{
    ThriftTransport parent;

    /* private */
    gchar * hostname;
    gshort port;

    int sd;
    guint8 * buf;
    guint32 buf_size;
    guint32 buf_len;
};

typedef struct _ThriftSocketClass ThriftSocketClass;
struct _ThriftSocketClass
{
    ThriftTransportClass parent;
};

GType thrift_socket_get_type (void);

#define THRIFT_TYPE_SOCKET (thrift_socket_get_type ())
#define THRIFT_SOCKET(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), THRIFT_TYPE_SOCKET, ThriftSocket))
#define THRIFT_SOCKET_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), THRIFT_TYPE_SOCKET, ThriftSocketClass))
#define THRIFT_IS_SOCKET(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), THRIFT_TYPE_SOCKET))
#define THRIFT_IS_SOCKET_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), THRIFT_TYPE_SOCKET))
#define THRIFT_SOCKET_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), THRIFT_TYPE_SOCKET, ThriftSocketClass))

typedef enum
{
    THRIFT_SOCKET_ERROR_HOST,
    THRIFT_SOCKET_ERROR_SOCKET,
    THRIFT_SOCKET_ERROR_CONNECT,
    THRIFT_SOCKET_ERROR_SEND,
    THRIFT_SOCKET_ERROR_RECEIVE
} ThriftSocketError;

GQuark thrift_socket_error_quark (void);
#define THRIFT_SOCKET_ERROR (thrift_socket_error_quark ())

#endif /* THRIFT_SOCKET_H */
