/**
 * Thrift c
 *
 */

/** TODO:
 * - look at utf8 support ???
 * - do we need strict_(write|read) support
 */

#include "thrift.h"
#include "protocol/thrift_binary_protocol.h"

#include <string.h>

gint32 _thrift_binary_protocol_write_message_begin (ThriftProtocol * protocol,
                                                    const gchar * name,
                                                    const ThriftMessageType message_type,
                                                    const gint32 seqid,
                                                    GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint32 version = (VERSION_1) | ((gint32)message_type);
    gint32 ret;
    gint32 xfer = 0;
    if ((ret = thrift_protocol_write_i32 (protocol, version, error)) < 0)
        return -1;
    xfer += ret;
    if ((ret = thrift_protocol_write_string (protocol, name, error)) < 0)
        return -1;
    xfer += ret;
    if ((ret = thrift_protocol_write_i32 (protocol, seqid, error)) < 0)
        return -1;
    xfer += ret;
    return xfer;
}

gint32 _thrift_binary_protocol_write_message_end (ThriftProtocol * protocol,
                                                  GError ** error)
{
    THRIFT_UNUSED_VAR (protocol);
    THRIFT_UNUSED_VAR (error);
    return 0;
}

gint32 _thrift_binary_protocol_write_struct_begin (ThriftProtocol * protocol,
                                                   const gchar * name,
                                                   GError ** error)
{
    THRIFT_UNUSED_VAR (protocol);
    THRIFT_UNUSED_VAR (name);
    THRIFT_UNUSED_VAR (error);
    return 0;
}

gint32 _thrift_binary_protocol_write_struct_end (ThriftProtocol * protocol,
                                                 GError ** error)
{
    THRIFT_UNUSED_VAR (protocol);
    THRIFT_UNUSED_VAR (error);
    return 0;
}

gint32 _thrift_binary_protocol_write_field_begin (ThriftProtocol * protocol,
                                                  const gchar * name,
                                                  const ThriftType field_type,
                                                  const gint16 field_id,
                                                  GError ** error)
{
    THRIFT_UNUSED_VAR (name);
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint32 ret;
    gint32 xfer = 0;
    if ((ret = thrift_protocol_write_byte (protocol, (gint8)field_type,
                                           error)) < 0)
        return -1;
    xfer += ret;
    if ((ret = thrift_protocol_write_i16 (protocol, field_id, error)) < 0)
        return -1;
    xfer += ret;
    return xfer;
}

gint32 _thrift_binary_protocol_write_field_end (ThriftProtocol * protocol,
                                                GError ** error)
{
    THRIFT_UNUSED_VAR (protocol);
    THRIFT_UNUSED_VAR (error);
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

    gint32 ret;
    gint32 xfer = 0;
    if ((ret = thrift_protocol_write_byte (protocol, (gint8)key_type,
                                           error)) < 0)
        return -1;
    xfer += ret;
    if ((ret = thrift_protocol_write_byte (protocol, (gint8)value_type,
                                           error)) < 0)
        return -1;
    xfer += ret;
    if ((ret = thrift_protocol_write_i32 (protocol, (gint32)size, error)) < 0)
        return -1;
    xfer += ret;
    return xfer;
}

gint32 _thrift_binary_protocol_write_map_end (ThriftProtocol * protocol,
                                              GError ** error)
{
    THRIFT_UNUSED_VAR (protocol);
    THRIFT_UNUSED_VAR (error);
    return 0;
}

gint32 _thrift_binary_protocol_write_list_begin (ThriftProtocol * protocol,
                                                 const ThriftType element_type,
                                                 const guint32 size,
                                                 GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint32 ret;
    gint32 xfer = 0;
    if ((ret = thrift_protocol_write_byte (protocol, (gint8)element_type,
                                           error)) < 0)
        return -1;
    xfer += ret;
    if ((ret = thrift_protocol_write_i32 (protocol, (gint32)size, error)) < 0)
        return -1;
    xfer += ret;
    return xfer;
}

gint32 _thrift_binary_protocol_write_list_end (ThriftProtocol * protocol,
                                               GError ** error)
{
    THRIFT_UNUSED_VAR (protocol);
    THRIFT_UNUSED_VAR (error);
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
    THRIFT_UNUSED_VAR (protocol);
    THRIFT_UNUSED_VAR (error);
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

    return thrift_transport_write (protocol->transport, (const gpointer)&byte, 
                                   1, error);
}

gint32 _thrift_binary_protocol_write_i16 (ThriftProtocol * protocol,
                                          const gint16 i16, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint16 net = g_htons (i16);
    return thrift_transport_write (protocol->transport, (const gpointer)&net, 
                                   2, error);
}

gint32 _thrift_binary_protocol_write_i32 (ThriftProtocol * protocol,
                                          const gint32 i32, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint32 net = g_htonl (i32);
    return thrift_transport_write (protocol->transport, (const gpointer)&net, 
                                   4, error);
}

gint32 _thrift_binary_protocol_write_i64 (ThriftProtocol * protocol,
                                          const gint64 i64, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    /* g_htonll hasn't been added yet, patch submitted, just macro anyway */
    gint64 net = GUINT64_TO_BE (i64);
    return thrift_transport_write (protocol->transport, (const gpointer)&net, 
                                   8, error);
}

gint32 _thrift_binary_protocol_write_double (ThriftProtocol * protocol,
                                             const gdouble dub, 
                                             GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));
    g_assert (sizeof (gdouble) == sizeof (guint64));

    guint64 bits = GUINT64_FROM_BE (*(unsigned long long *)&dub);
    return thrift_transport_write (protocol->transport, (const gpointer)&bits, 
                                   8, error);
    return 8;
}

gint32 _thrift_binary_protocol_write_string (ThriftProtocol * protocol,
                                             const gchar * str, 
                                             GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    guint32 len = str != NULL ? strlen (str) : 0;
    /* TODO: we need to utf8 encode... */
    return thrift_protocol_write_binary (protocol, str, len, error);
}

gint32 _thrift_binary_protocol_write_binary (ThriftProtocol * protocol,
                                             const char * buf,
                                             const guint32 len,
                                             GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint32 ret;
    gint32 xfer = 0;
    if ((ret = thrift_protocol_write_i32 (protocol, len, error)) < 0)
        return -1;
    xfer += ret;
    if (len > 0)
    {
        if ((ret = thrift_transport_write (protocol->transport, 
                                           (const gpointer)buf, len,
                                           error)) < 0)
            return -1;
        xfer += ret;
    }
    return xfer;
}

gint32 _thrift_binary_protocol_read_message_begin (ThriftProtocol * protocol,
                                                   gchar ** name,
                                                   ThriftMessageType * message_type,
                                                   gint32 * seq_id,
                                                   GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint32 ret;
    gint32 xfer = 0;
    gint32 sz;
    if ((ret = thrift_protocol_read_i32 (protocol, &sz, error)) < 0)
        return -1;
    xfer += ret;

    if (sz < 0)
    {
        /* Check for correct version number */
        guint32 version = sz & VERSION_MASK;
        if (version != VERSION_1)
        {
            g_set_error (error, THRIFT_BINARY_PROTOCOL_ERROR,
                         THRIFT_BINARY_PROTOCOL_ERROR_WRONG_VERSION,
                         "expected %d, got %d", VERSION_1, version);
            return -1;
        }
        /* TODO: shouldn't this be xor with VERSION_MASK or something */
        *message_type = (ThriftMessageType)(sz & 0x000000ff);
        if ((ret = thrift_protocol_read_string (protocol, name, error)) < 0)
            return -1;
        xfer += ret;
        if ((ret = thrift_protocol_read_i32 (protocol, seq_id, error)) < 0)
            return -1;
        xfer += ret;
    }

    return xfer;
}

gint32 _thrift_binary_protocol_read_message_end (ThriftProtocol * protocol,
                                                 GError ** error)
{
    THRIFT_UNUSED_VAR (protocol);
    THRIFT_UNUSED_VAR (error);
    return 0;
}

gint32 _thrift_binary_protocol_read_struct_begin (ThriftProtocol * protocol,
                                                  gchar ** name,
                                                  GError ** error)
{
    THRIFT_UNUSED_VAR (protocol);
    THRIFT_UNUSED_VAR (error);
    *name = NULL;
    return 0;
}

gint32 _thrift_binary_protocol_read_struct_end (ThriftProtocol * protocol,
                                                GError ** error)
{
    THRIFT_UNUSED_VAR (protocol);
    THRIFT_UNUSED_VAR (error);
    return 0;
}

gint32 _thrift_binary_protocol_read_field_begin (ThriftProtocol * protocol,
                                                 gchar ** name,
                                                 ThriftType * field_type,
                                                 gint16 * field_id,
                                                 GError ** error)
{
    THRIFT_UNUSED_VAR (name);
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint32 ret;
    gint32 xfer = 0;
    gint8 type;
    if ((ret = thrift_protocol_read_byte (protocol, &type, error)) < 0)
        return -1;
    xfer += ret;
    *field_type = (ThriftType)type;
    if (*field_type == T_STOP)
    {
        *field_id = 0;
        return xfer;
    }
    if ((ret = thrift_protocol_read_i16 (protocol, field_id, error)) < 0)
        return -1;
    xfer += ret;
    return xfer;
}

gint32 _thrift_binary_protocol_read_field_end (ThriftProtocol * protocol,
                                               GError ** error)
{
    THRIFT_UNUSED_VAR (protocol);
    THRIFT_UNUSED_VAR (error);
    return 0;
}

gint32 _thrift_binary_protocol_read_map_begin (ThriftProtocol * protocol,
                                               ThriftType * key_type,
                                               ThriftType * value_type,
                                               guint32 * size, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint32 ret;
    gint32 xfer = 0;
    gint8 k, v;
    gint32 sizei;
    if ((ret = thrift_protocol_read_byte (protocol, &k, error)) < 0)
        return -1;
    xfer += ret;
    *key_type = (ThriftType)k;
    if ((ret = thrift_protocol_read_byte (protocol, &v, error)) < 0)
        return -1;
    xfer += ret;
    *value_type = (ThriftType)v;
    if ((ret = thrift_protocol_read_i32 (protocol, &sizei, error)) < 0)
        return -1;
    xfer += ret;
    if (sizei < 0)
    {
        g_set_error (error, THRIFT_BINARY_PROTOCOL_ERROR, 
                     THRIFT_BINARY_PROTOCOL_ERROR_NEGATIVE_SIZE,
                     "size of %d", sizei);
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
    return xfer;
}

gint32 _thrift_binary_protocol_read_map_end (ThriftProtocol * protocol,
                                             GError ** error)
{
    THRIFT_UNUSED_VAR (protocol);
    THRIFT_UNUSED_VAR (error);
    return 0;
}

gint32 _thrift_binary_protocol_read_list_begin (ThriftProtocol * protocol,
                                                ThriftType * element_type,
                                                guint32 * size,
                                                GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint32 ret;
    gint32 xfer = 0;
    gint8 e;
    gint32 sizei;
    if ((ret = thrift_protocol_read_byte (protocol, &e, error)) < 0)
        return -1;
    xfer += ret;
    *element_type = (ThriftType)e;
    if ((ret = thrift_protocol_read_i32 (protocol, &sizei, error)) < 0)
        return -1;
    xfer += ret;
    if (sizei < 0)
    {
        g_set_error (error, THRIFT_BINARY_PROTOCOL_ERROR, 
                     THRIFT_BINARY_PROTOCOL_ERROR_NEGATIVE_SIZE,
                     "size of %d", sizei);
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
    return xfer;
}

gint32 _thrift_binary_protocol_read_list_end (ThriftProtocol * protocol,
                                              GError ** error)
{
    THRIFT_UNUSED_VAR (protocol);
    THRIFT_UNUSED_VAR (error);
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
    THRIFT_UNUSED_VAR (protocol);
    THRIFT_UNUSED_VAR (error);
    return 0;
}

gint32 _thrift_binary_protocol_read_bool (ThriftProtocol * protocol,
                                          gboolean * value, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint32 ret;
    gpointer b[1];
    if ((ret = thrift_transport_read (protocol->transport, b, 1, error)) < 0)
        return -1;
    *value = *(gint8*)b != 0;
    return ret;
}

gint32 _thrift_binary_protocol_read_byte (ThriftProtocol * protocol,
                                          gint8 * byte, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint32 ret;
    gpointer b[1];
    if ((ret = thrift_transport_read (protocol->transport, b, 1, error)) < 0)
        return -1;
    *byte = *(gint8*)b;
    return ret;
}

gint32 _thrift_binary_protocol_read_i16 (ThriftProtocol * protocol,
                                         gint16 * i16, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint32 ret;
    gpointer b[2];
    if ((ret = thrift_transport_read (protocol->transport, b, 2, error)) < 0)
        return -1;
    *i16 = *(gint16*)b;
    *i16 = g_ntohs (*i16);
    return ret;
}

gint32 _thrift_binary_protocol_read_i32 (ThriftProtocol * protocol,
                                         gint32 * i32, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint32 ret;
    gpointer b[4];
    if ((ret = thrift_transport_read (protocol->transport, b, 4, error)) < 0)
        return -1;
    *i32 = *(gint32*)b;
    *i32 = g_ntohl (*i32);
    return ret;
}

gint32 _thrift_binary_protocol_read_i64 (ThriftProtocol * protocol,
                                         gint64 * i64, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint32 ret;
    gpointer b[8];
    if ((ret = thrift_transport_read (protocol->transport, b, 8, error)) < 0)
        return -1;
    *i64 = *(gint64*)b;
    *i64 = GUINT64_FROM_BE (*i64);
    return ret;
}

gint32 _thrift_binary_protocol_read_double (ThriftProtocol * protocol,
                                            gdouble * dub, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));
    g_assert (sizeof (gdouble) == sizeof (guint64));

    gint32 ret;
    gpointer b[8];
    if ((ret = thrift_transport_read (protocol->transport, b, 8, error)) < 0)
        return -1;
    guint64 bits = *(guint64*)b;
    bits = GUINT64_FROM_BE (bits);
    *dub = *(gdouble*)&bits;
    return ret;
}

gint32 _thrift_binary_protocol_read_string (ThriftProtocol * protocol,
                                            gchar ** str, GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    guint32 len;
    return thrift_protocol_read_binary (protocol, str, &len, error);
}

gint32 _thrift_binary_protocol_read_binary (ThriftProtocol * protocol,
                                            gchar ** buf, guint32 * len,
                                            GError ** error)
{
    g_assert (THRIFT_IS_BINARY_PROTOCOL (protocol));

    gint32 ret;
    gint32 xfer = 0;

    if ((ret = thrift_protocol_read_i32 (protocol, (gint32*)len, error)) < 0)
        return -1;
    xfer += ret;

    if (*len > 0)
    {
      *buf= g_new (gchar, *len + 1);
      if ((ret = thrift_transport_read (protocol->transport, *buf, *len,
                                        error)) < 0)
          return -1;
      xfer += ret;
      (*buf)[*len] = '\0';
    }
    else
    {
      *buf = NULL;
    }

    return xfer;
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
