/**
 * Thrift c 
 *
 */
#ifndef THRIFT_TRANSPORT_H
#define THRIFT_TRANSPORT_H

#include <glib-object.h>

typedef struct _ThriftTransport ThriftTransport;
struct _ThriftTransport
{ 
    GObject parent; 

    /* private */
}; 

typedef struct _ThriftTransportClass ThriftTransportClass;
struct _ThriftTransportClass
{ 
    GObjectClass parent; 

    gboolean (*is_open) (ThriftTransport * transport);
    gboolean (*open) (ThriftTransport * transport, GError ** error);
    gboolean (*close) (ThriftTransport * transport, GError ** error);
    gint32 (*read) (ThriftTransport * transport, gpointer buf, 
                    guint32 len, GError ** error);
    gboolean (*read_end) (ThriftTransport * transport, GError ** error);
    gint32 (*write) (ThriftTransport * transport, const gpointer buf, 
                     const guint32 len, GError ** error);
    gboolean (*write_end) (ThriftTransport * transport, GError ** error);
    gboolean (*flush) (ThriftTransport * transport, GError ** error);
}; 

GType thrift_transport_get_type (void);

#define THRIFT_TYPE_TRANSPORT (thrift_transport_get_type ())
#define THRIFT_TRANSPORT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), THRIFT_TYPE_TRANSPORT, ThriftTransport))
#define THRIFT_TRANSPORT_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), THRIFT_TYPE_TRANSPORT, ThriftTransportClass))
#define THRIFT_IS_TRANSPORT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), THRIFT_TYPE_TRANSPORT))
#define THRIFT_IS_TRANSPORT_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), THRIFT_TYPE_TRANSPORT))
#define THRIFT_TRANSPORT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), THRIFT_TYPE_TRANSPORT, ThriftTransportClass))

typedef enum
{
    THRIFT_TRANSPORT_ERROR_NOT_SUPPORTED
} ThriftTransportError;

GQuark thrift_transport_error_quark (void);
#define THRIFT_TRANSPORT_ERROR (thrift_transport_error_quark ())

gboolean thrift_transport_is_open (ThriftTransport * transport);
gboolean thrift_transport_open (ThriftTransport * transport, GError ** error);
gboolean thrift_transport_close (ThriftTransport * transport, GError ** error);
gint32 thrift_transport_read (ThriftTransport * transport, gpointer buf, 
                              guint32 len, GError ** error);
gboolean thrift_transport_read_end (ThriftTransport * transport, 
                                    GError ** error);
gint32 thrift_transport_write (ThriftTransport * transport, const gpointer buf, 
                               const guint32 len, GError ** error);
gboolean thrift_transport_write_end (ThriftTransport * transport, 
                                     GError ** error);
gboolean thrift_transport_flush (ThriftTransport * transport, GError ** error);


#endif /* THRIFT_TRANSPORT_H */
