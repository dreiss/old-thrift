/**
 * Thrift c 
 *
 */
#ifndef THRIFT_SOCKET_H
#define THRIFT_SOCKET_H

#include <glib-object.h>

typedef struct _ThriftSocket ThriftSocket;
struct _ThriftSocket
{ 
    GObject parent; 

    /* private */
    gchar * hostname;
    gshort port;
    int socket;
}; 

typedef struct _ThriftSocketClass ThriftSocketClass;
struct _ThriftSocketClass
{ 
    GObjectClass parent; 
}; 

GType thrift_socket_get_type (void);

#define THRIFT_TYPE_SOCKET (thrift_socket_get_type ())
#define THRIFT_SOCKET(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), THRIFT_TYPE_SOCKET, ThriftSocket))
#define THRIFT_SOCKET_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), THRIFT_TYPE_SOCKET, ThriftSocketClass))
#define THRIFT_IS_SOCKET(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), THRIFT_TYPE_SOCKET))
#define THRIFT_IS_SOCKET_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), THRIFT_TYPE_SOCKET))
#define THRIFT_SOCKET_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), THRIFT_TYPE_SOCKET, ThriftSocketClass))

gboolean thrift_socket_connect (ThriftSocket * socket);
gint thrift_socket_send (ThriftSocket * socket, const gpointer buf, guint len);
gint thrift_socket_receive (ThriftSocket * socket, gpointer buf, guint len);

#endif /* THRIFT_SOCKET_H */
