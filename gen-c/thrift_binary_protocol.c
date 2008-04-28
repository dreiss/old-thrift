/**
 * Thrift c
 *
 */

/** TODO:
 * - this is really binary protocol, it needs to be moved...
 * - look at using glib's byte order stuffs
 * - look at utf8 support ???
 * - error handling, gerror or whatever
 * - do we need strict_(write|read) support
 */

#include "thrift_binary_protocol.h"

#include <string.h>

gint32 _thrift_binary_protocol_write_message_begin (ThriftProtocol * protocol,
                                                    const gchar * name,
                                                    const ThriftMessageType message_type,
                                                    const gint32 seqid,
                                                    GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint32 version = (VERSION_1) | ((gint32)message_type);
    guint32 wsize = 0;
    wsize += thrift_protocol_write_i32 (protocol, version, error);
    wsize += thrift_protocol_write_string (protocol, name, error);
    wsize += thrift_protocol_write_i32 (protocol, seqid, error);
    return wsize;
}

gint32 _thrift_binary_protocol_write_message_end (ThriftProtocol * protocol,
                                                  GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    thrift_transport_flush (protocol->transport);
    return 0;
}

gint32 _thrift_binary_protocol_write_struct_begin (ThriftProtocol * protocol,
                                                   const gchar * name,
                                                   GError ** error)
{
    return 0;
}

gint32 _thrift_binary_protocol_write_struct_end (ThriftProtocol * protocol,
                                                 GError ** error)
{
    return 0;
}

gint32 _thrift_binary_protocol_write_field_begin (ThriftProtocol * protocol,
                                                  const gchar * name,
                                                  const ThriftType field_type,
                                                  const gint16 field_id,
                                                  GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    guint32 wsize = 0;
    wsize += thrift_protocol_write_byte (protocol, (gint8)field_type,
                                         error);
    wsize += thrift_protocol_write_i16 (protocol, field_id, error);
    return wsize;
}

gint32 _thrift_binary_protocol_write_field_end (ThriftProtocol * protocol,
                                                GError ** error)
{
    return 0;
}

gint32 _thrift_binary_protocol_write_field_stop (ThriftProtocol * protocol,
                                                 GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    return thrift_protocol_write_byte (protocol, (gint8)T_STOP, error);
}

gint32 _thrift_binary_protocol_write_map_begin (ThriftProtocol * protocol,
                                                const ThriftType key_type,
                                                const ThriftType value_type,
                                                const guint32 size,
                                                GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    guint32 wsize = 0;
    wsize += thrift_protocol_write_byte (protocol, (gint8)key_type,
                                         error);
    wsize += thrift_protocol_write_byte (protocol, (gint8)value_type,
                                         error);
    wsize += thrift_protocol_write_i32 (protocol, (gint32)size, error);
    return wsize;
}

gint32 _thrift_binary_protocol_write_map_end (ThriftProtocol * protocol,
                                              GError ** error)
{
    return 0;
}

gint32 _thrift_binary_protocol_write_list_begin (ThriftProtocol * protocol,
                                                 const ThriftType element_type,
                                                 const guint32 size,
                                                 GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    guint32 wsize = 0;
    wsize += thrift_protocol_write_byte (protocol, (gint8)element_type,
                                         error);
    wsize += thrift_protocol_write_i32 (protocol, (gint32)size, error);
    return wsize;
}

gint32 _thrift_binary_protocol_write_list_end (ThriftProtocol * protocol,
                                               GError ** error)
{
    return 0;
}

gint32 _thrift_binary_protocol_write_set_begin (ThriftProtocol * protocol,
                                                const ThriftType element_type,
                                                const guint32 size,
                                                GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    return thrift_protocol_write_list_begin (protocol, element_type,
                                             size, error);
}

gint32 _thrift_binary_protocol_write_set_end (ThriftProtocol * protocol,
                                              GError ** error)
{
    return 0;
}

gint32 _thrift_binary_protocol_write_bool (ThriftProtocol * protocol,
                                           const gboolean value,
                                           GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    guint8 tmp = value ? 1 : 0;
    return thrift_protocol_write_byte (protocol, tmp, error);
}

gint32 _thrift_binary_protocol_write_byte (ThriftProtocol * protocol,
                                           const gint8 byte, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    thrift_transport_write (protocol->transport, (const gpointer)&byte, 1,
                            error);
    return 1;
}

gint32 _thrift_binary_protocol_write_i16 (ThriftProtocol * protocol,
                                          const gint16 i16, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint16 net = g_htons (i16);
    thrift_transport_write (protocol->transport, (const gpointer)&net, 2,
                            error);
    return 2;
}

gint32 _thrift_binary_protocol_write_i32 (ThriftProtocol * protocol,
                                          const gint32 i32, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint32 net = g_htonl (i32);
    thrift_transport_write (protocol->transport, (const gpointer)&net, 4,
                            error);
    return 4;
}

gint32 _thrift_binary_protocol_write_i64 (ThriftProtocol * protocol,
                                          const gint64 i64, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));
    g_assert_not_reached ();

    /* TODO: 64-bit htonll??? */
    gint64 net = (gint64)g_htonl (i64);
    thrift_transport_write (protocol->transport, (const gpointer)&net, 8,
                            error);
    return 8;
}

gint32 _thrift_binary_protocol_write_double (ThriftProtocol * protocol,
                                             const double dub, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));
    g_assert_not_reached ();

    /* TODO: is the "cast" enough? */
    return thrift_protocol_write_i64 (protocol, dub, error);
}

gint32 _thrift_binary_protocol_write_string (ThriftProtocol * protocol,
                                             const gchar * str, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    guint32 len = strlen (str);
    /* TODO: we need to utf8 encode... */
    return thrift_protocol_write_binary (protocol, str, len, error);
}

gint32 _thrift_binary_protocol_write_binary (ThriftProtocol * protocol,
                                             const char * buf,
                                             const guint32 len,
                                             GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    guint32 result = thrift_protocol_write_i32 (protocol, len, error);
    if (len > 0)
        thrift_transport_write (protocol->transport, (const gpointer)buf, len,
                                error);
    return result + len;
}

gint32 _thrift_binary_protocol_read_message_begin (ThriftProtocol * protocol,
                                                   gchar ** name,
                                                   ThriftMessageType * message_type,
                                                   gint32 * seq_id,
                                                   GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    guint32 result = 0;
    gint32 sz;
    result += thrift_protocol_read_i32 (protocol, &sz, error);

    if (sz < 0)
    {
        /* Check for correct version number */
        gint32 version = sz & VERSION_MASK;
        /* TODO: error handling */
        if (version != VERSION_1)
            return -1;
        /* TODO: shouldn't this be xor with VERSION_MASK or something */
        *message_type = (ThriftMessageType)(sz & 0x000000ff);
        result += thrift_protocol_read_string (protocol, name, error);
        result += thrift_protocol_read_i32 (protocol, seq_id, error);
    }

    return result;
}

gint32 _thrift_binary_protocol_read_message_end (ThriftProtocol * protocol,
                                                 GError ** error)
{
    return 0;
}

gint32 _thrift_binary_protocol_read_struct_begin (ThriftProtocol * protocol,
                                                  gchar ** name,
                                                  GError ** error)
{
    *name = NULL;
}

gint32 _thrift_binary_protocol_read_struct_end (ThriftProtocol * protocol,
                                                GError ** error)
{
    return 0;
}

gint32 _thrift_binary_protocol_read_field_begin (ThriftProtocol * protocol,
                                                 gchar ** name,
                                                 ThriftType * field_type,
                                                 gint16 * field_id,
                                                 GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    guint32 result = 0;
    gint8 type;
    result += thrift_protocol_read_byte (protocol, &type, error);
    *field_type = (ThriftType)type;
    if (*field_type == T_STOP)
    {
        *field_id = 0;
        return result;
    }
    result += thrift_protocol_read_i16 (protocol, field_id, error);
    return result;
}

gint32 _thrift_binary_protocol_read_field_end (ThriftProtocol * protocol,
                                               GError ** error)
{
    return 0;
}

gint32 _thrift_binary_protocol_read_map_begin (ThriftProtocol * protocol,
                                               ThriftType * key_type,
                                               ThriftType * value_type,
                                               guint32 * size, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint8 k, v;
    guint32 result = 0;
    gint32 sizei;
    result += thrift_protocol_read_byte (protocol, &k, error);
    *key_type = (ThriftType)k;
    result += thrift_protocol_read_byte (protocol, &v, error);
    *value_type = (ThriftType)v;
    result += thrift_protocol_read_i32 (protocol, &sizei, error);
    if (sizei < 0)
    {
        /* TODO: error handling */
        return -1;
    }
#if 0
TODO: container limit???
    else if (container_limit_ && sizei > container_limit_)
    {
        /* TODO: error handling */
        return -1;
    }
#endif
    *size = (guint32)sizei;
    return result;
}

gint32 _thrift_binary_protocol_read_map_end (ThriftProtocol * protocol,
                                             GError ** error)
{
    return 0;
}

gint32 _thrift_binary_protocol_read_list_begin (ThriftProtocol * protocol,
                                                ThriftType * element_type,
                                                guint32 * size,
                                                GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint8 e;
    guint32 result = 0;
    gint32 sizei;
    result += thrift_protocol_read_byte (protocol, &e, error);
    *element_type = (ThriftType)e;
    result += thrift_protocol_read_i32 (protocol, &sizei, error);
    if (sizei < 0)
    {
        /* TODO: error handling */
        return -1;
    }
#if 0
TODO: container limit???
    else if (container_limit_ && sizei > container_limit_)
    {
        /* TODO: error handling */
        return -1;
    }
#endif
    *size = (guint32)sizei;
    return result;
}

gint32 _thrift_binary_protocol_read_list_end (ThriftProtocol * protocol,
                                              GError ** error)
{
    return 0;
}

gint32 _thrift_binary_protocol_read_set_begin (ThriftProtocol * protocol,
                                               ThriftType * element_type,
                                               guint32 * size, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    return thrift_protocol_read_list_begin (protocol, element_type,
                                            size, error);
}

gint32 _thrift_binary_protocol_read_set_end (ThriftProtocol * protocol,
                                             GError ** error)
{
    return 0;
}

gint32 _thrift_binary_protocol_read_bool (ThriftProtocol * protocol,
                                          gboolean * value, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gpointer b[1];
    thrift_transport_read (protocol->transport, b, 1, error);
    *value = *(gint8*)b != 0;
    return 1;
}

gint32 _thrift_binary_protocol_read_byte (ThriftProtocol * protocol,
                                          gint8 * byte, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gpointer b[1];
    thrift_transport_read (protocol->transport, b, 1, error);
    *byte = *(gint8*)b;
    return 1;
}

gint32 _thrift_binary_protocol_read_i16 (ThriftProtocol * protocol,
                                         gint16 * i16, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gpointer b[2];
    thrift_transport_read (protocol->transport, b, 2, error);
    *i16 = *(gint16*)b;
    *i16 = g_ntohs (*i16);
    return 2;
}

gint32 _thrift_binary_protocol_read_i32 (ThriftProtocol * protocol,
                                         gint32 * i32, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gpointer b[4];
    thrift_transport_read (protocol->transport, b, 4, error);
    *i32 = *(gint32*)b;
    *i32 = g_ntohl (*i32);
    return 4;
}

gint32 _thrift_binary_protocol_read_i64 (ThriftProtocol * protocol,
                                         gint64 * i64, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gpointer b[8];
    thrift_transport_read (protocol->transport, b, 8, error);
    *i64 = *(gint64*)b;
    /* TODO: 64-bit htonll??? */
    *i64 = g_ntohl (*i64);
    return 8;
}

gint32 _thrift_binary_protocol_read_double (ThriftProtocol * protocol,
                                            double * dub, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    /* TODO: is the cast enough? */
    gint32 result = thrift_protocol_read_i64 (protocol, (gint64*)dub,
                                              error);
    return result;
}

gint32 _thrift_binary_protocol_read_string (ThriftProtocol * protocol,
                                            gchar ** str, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    guint32 len;
    guint32 result = thrift_protocol_read_binary (protocol, str, &len,
                                                  error);
    return result;
}

gint32 _thrift_binary_protocol_read_binary (ThriftProtocol * protocol,
                                            gchar ** str, guint32 * len,
                                            GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint32 result = thrift_protocol_read_i32 (protocol, len, error);

    *str = g_new (gchar, *len);
    /* TODO: make sure we got the mem */
    gint ret = thrift_transport_read (protocol->transport, *str, *len,
                                      error);
    if (ret < 0 || ret != *len)
    {
        /* TODO: error handling */
        return -1;
    }

    return result + *len;
}

enum _ThriftBinaryProtocolProperties
{
    PROP_DUMMY,
};

static void _thrift_binary_protocol_instance_init (ThriftBinaryProtocol * protocol)
{
    protocol->transport = NULL;
}

static void _thrift_binary_protocol_class_init (ThriftProtocolClass * thrift_protocol_class)
{
    thrift_protocol_class->write_message_begin = _thrift_binary_protocol_write_message_begin;
    thrift_protocol_class->write_message_end = _thrift_binary_protocol_write_message_end;
    thrift_protocol_class->write_struct_begin = _thrift_binary_protocol_write_struct_begin;
    thrift_protocol_class->write_struct_end = _thrift_binary_protocol_write_struct_end;
    thrift_protocol_class->write_field_begin = _thrift_binary_protocol_write_field_begin;
    thrift_protocol_class->write_field_end = _thrift_binary_protocol_write_field_end;
    thrift_protocol_class->write_field_stop = _thrift_binary_protocol_write_field_stop;
    thrift_protocol_class->write_map_begin = _thrift_binary_protocol_write_map_begin;
    thrift_protocol_class->write_map_end = _thrift_binary_protocol_write_map_end;
    thrift_protocol_class->write_list_begin = _thrift_binary_protocol_write_list_begin;
    thrift_protocol_class->write_list_end = _thrift_binary_protocol_write_list_end;
    thrift_protocol_class->write_set_begin = _thrift_binary_protocol_write_set_begin;
    thrift_protocol_class->write_set_end = _thrift_binary_protocol_write_set_end;
    thrift_protocol_class->write_bool = _thrift_binary_protocol_write_bool;
    thrift_protocol_class->write_byte = _thrift_binary_protocol_write_byte;
    thrift_protocol_class->write_i16 = _thrift_binary_protocol_write_i16;
    thrift_protocol_class->write_i32 = _thrift_binary_protocol_write_i32;
    thrift_protocol_class->write_i64 = _thrift_binary_protocol_write_i64;
    thrift_protocol_class->write_double = _thrift_binary_protocol_write_double;
    thrift_protocol_class->write_string = _thrift_binary_protocol_write_string;
    thrift_protocol_class->write_binary = _thrift_binary_protocol_write_binary;

    thrift_protocol_class->read_message_begin = _thrift_binary_protocol_read_message_begin;
    thrift_protocol_class->read_message_end = _thrift_binary_protocol_read_message_end;
    thrift_protocol_class->read_struct_begin = _thrift_binary_protocol_read_struct_begin;
    thrift_protocol_class->read_struct_end = _thrift_binary_protocol_read_struct_end;
    thrift_protocol_class->read_field_begin = _thrift_binary_protocol_read_field_begin;
    thrift_protocol_class->read_field_end = _thrift_binary_protocol_read_field_end;
    thrift_protocol_class->read_map_begin = _thrift_binary_protocol_read_map_begin;
    thrift_protocol_class->read_map_end = _thrift_binary_protocol_read_map_end;
    thrift_protocol_class->read_list_begin = _thrift_binary_protocol_read_list_begin;
    thrift_protocol_class->read_list_end = _thrift_binary_protocol_read_list_end;
    thrift_protocol_class->read_set_begin = _thrift_binary_protocol_read_set_begin;
    thrift_protocol_class->read_set_end = _thrift_binary_protocol_read_set_end;
    thrift_protocol_class->read_bool = _thrift_binary_protocol_read_bool;
    thrift_protocol_class->read_byte = _thrift_binary_protocol_read_byte;
    thrift_protocol_class->read_i16 = _thrift_binary_protocol_read_i16;
    thrift_protocol_class->read_i32 = _thrift_binary_protocol_read_i32;
    thrift_protocol_class->read_i64 = _thrift_binary_protocol_read_i64;
    thrift_protocol_class->read_double = _thrift_binary_protocol_read_double;
    thrift_protocol_class->read_string = _thrift_binary_protocol_read_string;
    thrift_protocol_class->read_binary = _thrift_binary_protocol_read_binary;
}

GType thrift_binary_protocol_get_type (void)
{
    static GType type = 0;

    if (type == 0)
    {
        static const GTypeInfo type_info =
        {
            sizeof (ThriftBinaryProtocolClass),
            NULL, /* base_init */
            NULL, /* base_finalize */
            (GClassInitFunc)_thrift_binary_protocol_class_init,
            NULL, /* class_finalize */
            NULL, /* class_data */
            sizeof (ThriftBinaryProtocol),
            0, /* n_preallocs */
            (GInstanceInitFunc)_thrift_binary_protocol_instance_init,
            NULL, /* value_table */
        };

        type = g_type_register_static (THRIFT_TYPE_PROTOCOL,
                                       "ThriftBinaryProtocolType",
                                       &type_info, 0);
    }

    return type;
}
