/**
 * Thrift c
 *
 */

#include "thrift.h"
#include "thrift_transport.h"

#include <netdb.h>
#include <string.h>

gboolean thrift_transport_is_open (ThriftTransport * transport)
{
    return THRIFT_TRANSPORT_GET_CLASS (transport)->is_open (transport);
}

gboolean thrift_transport_open (ThriftTransport * transport, GError ** error)
{
    return THRIFT_TRANSPORT_GET_CLASS (transport)->open (transport, error);
}

gboolean thrift_transport_close (ThriftTransport * transport, GError ** error)
{
    return THRIFT_TRANSPORT_GET_CLASS (transport)->close (transport, error);
}

gint32 thrift_transport_read (ThriftTransport * transport, gpointer buf, 
                              guint32 len, GError ** error)
{
    return THRIFT_TRANSPORT_GET_CLASS (transport)->read (transport, buf, len, 
                                                         error);
}

void thrift_transport_read_end (ThriftTransport * transport)
{
    return THRIFT_TRANSPORT_GET_CLASS (transport)->read_end (transport);
}

gint32 thrift_transport_write (ThriftTransport * transport, const gpointer buf, 
                               const guint32 len, GError ** error)
{
    return THRIFT_TRANSPORT_GET_CLASS (transport)->write (transport, buf, len, 
                                                          error);
}

void thrift_transport_write_end (ThriftTransport * transport)
{
    return THRIFT_TRANSPORT_GET_CLASS (transport)->write_end (transport);
}

void thrift_transport_flush (ThriftTransport * transport)
{
    THRIFT_TRANSPORT_GET_CLASS (transport)->flush (transport);
}

gboolean _thrift_transport_is_open (ThriftTransport * transport)
{
    THRIFT_UNUSED_VAR (transport);
    return 0;
}

gboolean _thrift_transport_open (ThriftTransport * transport, GError ** error)
{
    THRIFT_UNUSED_VAR (transport);
    THRIFT_UNUSED_VAR (error);
    g_assert_not_reached ();
    return 0;
}

gboolean _thrift_transport_close (ThriftTransport * transport, GError ** error)
{
    THRIFT_UNUSED_VAR (transport);
    THRIFT_UNUSED_VAR (error);
    g_assert_not_reached ();
    return 0;
}

gint32 _thrift_transport_read (ThriftTransport * transport, gpointer buf, 
                              guint32 len, GError ** error)
{
    THRIFT_UNUSED_VAR (transport);
    THRIFT_UNUSED_VAR (buf);
    THRIFT_UNUSED_VAR (len);
    THRIFT_UNUSED_VAR (error);
    g_assert_not_reached ();
    return 0;
}

/* TODO: add gerror's to the rest of the funcs like this one */
void _thrift_transport_read_end (ThriftTransport * transport)
{
    THRIFT_UNUSED_VAR (transport);
    return;
}

gint32 _thrift_transport_write (ThriftTransport * transport, const gpointer buf, 
                               const guint32 len, GError ** error)
{
    THRIFT_UNUSED_VAR (transport);
    THRIFT_UNUSED_VAR (buf);
    THRIFT_UNUSED_VAR (len);
    THRIFT_UNUSED_VAR (error);
    g_assert_not_reached ();
    return 0;
}

void _thrift_transport_write_end (ThriftTransport * transport)
{
    THRIFT_UNUSED_VAR (transport);
    return;
}

void _thrift_transport_flush (ThriftTransport * transport)
{
    THRIFT_UNUSED_VAR (transport);
    return;
}

enum _ThriftTransportProperties
{
    PROP_DUMMY
};

static void _thrift_transport_class_init (ThriftTransportClass * thrift_transport_class)
{
    thrift_transport_class->is_open = _thrift_transport_is_open;
    thrift_transport_class->open = _thrift_transport_open;
    thrift_transport_class->close = _thrift_transport_close;
    thrift_transport_class->read = _thrift_transport_read;
    thrift_transport_class->read_end = _thrift_transport_read_end;
    thrift_transport_class->write = _thrift_transport_write;
    thrift_transport_class->write_end = _thrift_transport_write_end;
    thrift_transport_class->flush = _thrift_transport_flush;
}

GQuark
thrift_transport_error_quark (void)
{
  return g_quark_from_static_string ("thrift-transport-error-quark");
}

GType thrift_transport_get_type (void)
{
    static GType type = 0;

    if (type == 0)
    {
        static const GTypeInfo type_info =
        {
            sizeof (ThriftTransportClass),
            NULL, /* base_init */
            NULL, /* base_finalize */
            (GClassInitFunc)_thrift_transport_class_init,
            NULL, /* class_finalize */
            NULL, /* class_data */
            sizeof (ThriftTransport),
            0, /* n_preallocs */
            NULL, /* instance_init */
            NULL, /* value_table */
        };

        type = g_type_register_static (G_TYPE_OBJECT,
                                       "ThriftTransportType",
                                       &type_info, 0);
    }

    return type;
}
