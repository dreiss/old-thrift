/**
 * Thrift c 
 *
 */
#ifndef THRIFT_FRAMED_H
#define THRIFT_FRAMED_H

#include <glib-object.h>

#include "thrift_transport.h"

typedef struct _ThriftFramed ThriftFramed;
struct _ThriftFramed
{ 
    ThriftTransport parent; 

    /* private */
    ThriftTransport * transport;

    guint8 * read_buf;
    guint32 read_buf_size;
    guint32 read_buf_pos;
    guint32 read_buf_len;

    guint8 * write_buf;
    guint32 write_buf_size;
    guint32 write_buf_len;
}; 

typedef struct _ThriftFramedClass ThriftFramedClass;
struct _ThriftFramedClass
{ 
    ThriftTransportClass parent; 
}; 

GType thrift_framed_get_type (void);

#define THRIFT_TYPE_FRAMED (thrift_framed_get_type ())
#define THRIFT_FRAMED(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), THRIFT_TYPE_FRAMED, ThriftFramed))
#define THRIFT_FRAMED_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), THRIFT_TYPE_FRAMED, ThriftFramedClass))
#define THRIFT_IS_FRAMED(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), THRIFT_TYPE_FRAMED))
#define THRIFT_IS_FRAMED_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), THRIFT_TYPE_FRAMED))
#define THRIFT_FRAMED_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), THRIFT_TYPE_FRAMED, ThriftFramedClass))

typedef enum
{
    THRIFT_FRAMED_ERROR_NEGATIVE_SIZE
} ThriftFramedError;

GQuark thrift_framed_error_quark (void);
#define THRIFT_FRAMED_ERROR (thrift_framed_error_quark ())

#endif /* THRIFT_FRAMED_H */
