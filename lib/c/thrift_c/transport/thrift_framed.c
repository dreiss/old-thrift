/**
 * Thrift c
 *
 */

#include "thrift.h"
#include "thrift_framed.h"

#include <netdb.h>
#include <string.h>

gboolean _thrift_framed_is_open (ThriftTransport * transport)
{
    ThriftFramed * framed = THRIFT_FRAMED (transport);
    return thrift_transport_is_open (framed->transport);
}

gboolean _thrift_framed_open (ThriftTransport * transport, GError ** error)
{
    ThriftFramed * framed = THRIFT_FRAMED (transport);
    return thrift_transport_open (framed->transport, error);
}

gboolean _thrift_framed_close (ThriftTransport * transport, GError ** error)
{
    ThriftFramed * framed = THRIFT_FRAMED (transport);
    return thrift_transport_close (framed->transport, error);
}

gint _thrift_framed_read_frame (ThriftFramed * framed, GError ** error)
{
    gint32 sz;

    // Get rid of the old frame
    if (framed->read_buf != NULL) 
    {
        g_free (framed->read_buf);
        framed->read_buf = NULL;
    }

    // Read in the next chunk size
    if (thrift_transport_read (framed->transport, &sz, 4, error) < 0)
        return -1;
    sz = g_ntohl (sz);

    if (sz < 0) 
    {
        g_set_error (error, THRIFT_FRAMED_ERROR, 
                     THRIFT_FRAMED_ERROR_NEGATIVE_SIZE, 
                     "frame size has negative value: %d", sz);
        return -1;
    }

    // Read the frame payload, reset markers
    framed->read_buf = g_new (guint8, sz);
    if (thrift_transport_read (framed->transport, framed->read_buf, sz, 
                               error) < 0)
        return -1;
    framed->read_buf_pos = 0;
    framed->read_buf_len = sz;

    return sz;
}

gint32 _thrift_framed_read (ThriftTransport * transport, gpointer buf, 
                            guint32 len, GError ** error)
{
    guint32 need = len;

    ThriftFramed * framed = THRIFT_FRAMED (transport);

    // We don't have enough data yet
    if ((framed->read_buf_len - framed->read_buf_pos) < need) 
    {
        // Copy out whatever we have
        if ((framed->read_buf_len  - framed->read_buf_pos) > 0) 
        {
            memcpy (buf, framed->read_buf + framed->read_buf_pos, 
                   framed->read_buf_len - framed->read_buf_pos);

            need -= framed->read_buf_len - framed->read_buf_pos;
            buf += framed->read_buf_len - framed->read_buf_pos;
        }

        // Read another chunk
        if (_thrift_framed_read_frame (framed, error) < 0)
            return -1;
    }

    // Hand over whatever we have
    guint32 give = need;
    if ((framed->read_buf_len - framed->read_buf_pos) < give) 
        give = framed->read_buf_len - framed->read_buf_pos;
    memcpy (buf, framed->read_buf + framed->read_buf_pos, give);
    framed->read_buf_pos += give;
    need -= give;
    return (len - need);
}

gboolean _thrift_framed_read_end (ThriftTransport * transport, GError ** error)
{
    ThriftFramed * framed = THRIFT_FRAMED (transport);
    return thrift_transport_read_end (framed->transport, error);
}

gint32 _thrift_framed_write (ThriftTransport * transport, const gpointer buf, 
                             const guint32 len, GError ** error)
{
    ThriftFramed * framed = THRIFT_FRAMED (transport);

    THRIFT_UNUSED_VAR (error);

    if (len == 0)
        return 0;

    // Need to grow the buffer
    if ((len + framed->write_buf_len) >= framed->write_buf_size)
    {
        guint32 new_size;

        // Double buffer size until sufficient
        while (new_size < (len + framed->write_buf_len))
            new_size *= 2;

        framed->write_buf = g_realloc (framed->write_buf, new_size);
        framed->write_buf_size = new_size;
    }

    // Copy data into buffer
    memcpy (framed->write_buf + framed->write_buf_len, buf, len);
    framed->write_buf_len += len;

    return len;
}

gboolean _thrift_framed_write_end (ThriftTransport * transport, 
                                   GError ** error)
{
    ThriftFramed * framed = THRIFT_FRAMED (transport);
    return thrift_transport_write_end (framed->transport, error);
}

gboolean _thrift_framed_flush (ThriftTransport * transport, GError ** error)
{
    ThriftFramed * framed = THRIFT_FRAMED (transport);
    gint32 sz = g_htonl (framed->write_buf_len);

    // Write frame size
    if (thrift_transport_write (framed->transport, &sz, 4, error) < 0)
        return 0;

    // Write frame body
    if (framed->write_buf_len > 0)
    {
        if (thrift_transport_write (framed->transport, framed->write_buf, 
                                framed->write_buf_len, error) < 0)
        {
            return 0;
        }
    }

    // All done
    framed->write_buf_len = 0;

    // Flush the underlying
    return thrift_transport_flush (framed->transport, error);
}

enum _ThriftFramedProperties
{
    PROP_DUMMY,
    PROP_TRANSPORT
};

void _thrift_framed_set_property (GObject * object, guint property_id,
                                  const GValue * value, GParamSpec * pspec)
{
    ThriftFramed * framed = THRIFT_FRAMED (object);

    THRIFT_UNUSED_VAR (pspec);

    /* TODO: we could check that pspec is the type we want, not sure that's nec
     * TODO: proper error here */
    switch (property_id)
    {
        case PROP_TRANSPORT:
            framed->transport = g_value_get_object (value);
            break;
    }
}

void _thrift_framed_get_property (GObject * object, guint property_id,
                                  GValue * value, GParamSpec * pspec)
{
    ThriftFramed * framed = THRIFT_FRAMED (object);

    THRIFT_UNUSED_VAR (pspec);

    /* TODO: we could check that pspec is the type we want, not sure that's nec
     * TODO: proper error here */
    switch (property_id)
    {
        case PROP_TRANSPORT:
            g_value_set_object (value, framed->transport);
            break;
    }
}

static void _thrift_framed_instance_init (ThriftFramed * framed)
{
    framed->transport = NULL;

    framed->read_buf_pos = 0;
    framed->read_buf_len = 0;
    framed->read_buf = NULL;

    framed->write_buf_size = 512;
    framed->write_buf_len = 0;
    framed->write_buf = g_new (guint8, framed->write_buf_size);
}

/* TODO: destroy/free */

static void _thrift_framed_class_init (ThriftTransportClass * transport_class)
{
    GObjectClass * gobject_class = G_OBJECT_CLASS (transport_class);
    GParamSpec * param_spec;

    gobject_class->set_property = _thrift_framed_set_property;
    gobject_class->get_property = _thrift_framed_get_property;

    param_spec = g_param_spec_object ("transport",
                                      "transport (construct)",
                                      "Set the transport of the framed trasport",
                                      THRIFT_TYPE_TRANSPORT,
                                      G_PARAM_CONSTRUCT_ONLY |
                                      G_PARAM_READWRITE);
    g_object_class_install_property (gobject_class, PROP_TRANSPORT, param_spec);

    transport_class->is_open = _thrift_framed_is_open;
    transport_class->open = _thrift_framed_open;
    transport_class->close = _thrift_framed_close;
    transport_class->read = _thrift_framed_read;
    transport_class->read_end = _thrift_framed_read_end;
    transport_class->write = _thrift_framed_write;
    transport_class->write_end = _thrift_framed_write_end;
    transport_class->flush = _thrift_framed_flush;
}

GQuark
thrift_framed_error_quark (void)
{
    return g_quark_from_static_string ("thrift-framed-error-quark");
}

GType thrift_framed_get_type (void)
{
    static GType type = 0;

    if (type == 0)
    {
        static const GTypeInfo type_info =
        {
            sizeof (ThriftFramedClass),
            NULL, /* base_init */
            NULL, /* base_finalize */
            (GClassInitFunc)_thrift_framed_class_init,
            NULL, /* class_finalize */
            NULL, /* class_data */
            sizeof (ThriftFramed),
            0, /* n_preallocs */
            (GInstanceInitFunc)_thrift_framed_instance_init,
            NULL, /* value_table */
        };

        type = g_type_register_static (THRIFT_TYPE_TRANSPORT,
                                       "ThriftFramedType",
                                       &type_info, 0);
    }

    return type;
}
