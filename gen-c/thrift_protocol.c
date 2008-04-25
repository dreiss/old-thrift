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

#include "thrift_protocol.h"

#include <string.h>

guint32 thrift_protocol_write_message_begin (ThriftProtocol * thrift_protocol,
                                             const gchar * name, 
                                             const ThriftMessageType message_type, 
                                             const gint32 seqid)
{
    g_assert (THRIFT_IS_PROTOCOL (thrift_protocol));

    gint32 version = (VERSION_1) | ((gint32)message_type);
    guint32 wsize = 0;
    wsize += thrift_protocol_write_i32 (thrift_protocol, version);
    wsize += thrift_protocol_write_string (thrift_protocol, name);
    wsize += thrift_protocol_write_i32 (thrift_protocol, seqid);
    return wsize;
}

guint32 thrift_protocol_write_message_end (ThriftProtocol * thrift_protocol)
{
    return 0;
}

guint32 thrift_protocol_write_struct_begin (ThriftProtocol * thrift_protocol,
                                            const gchar * name)
{
    return 0;
}

guint32 thrift_protocol_write_struct_end (ThriftProtocol * thrift_protocol)
{
    return 0;
}

guint32 thrift_protocol_write_field_begin (ThriftProtocol * thrift_protocol,
                                           const gchar * name,
                                           const ThriftType field_type,
                                           const gint16 field_id)
{
    g_assert (THRIFT_IS_PROTOCOL (thrift_protocol));

    guint32 wsize = 0;
    wsize += thrift_protocol_write_byte (thrift_protocol, (gint8)field_type);
    wsize += thrift_protocol_write_i16 (thrift_protocol, field_id);
    return wsize;
}

guint32 thrift_protocol_write_field_end (ThriftProtocol * thrift_protocol)
{
    return 0;
}

guint32 thrift_protocol_write_field_stop (ThriftProtocol * thrift_protocol)
{
    return thrift_protocol_write_byte (thrift_protocol, (gint8)T_STOP);
}

guint32 thrift_protocol_write_map_begin (ThriftProtocol * thrift_protocol,
                                         const ThriftType key_type,
                                         const ThriftType value_type,
                                         const guint32 size)
{
    g_assert (THRIFT_IS_PROTOCOL (thrift_protocol));

    guint32 wsize = 0;
    wsize += thrift_protocol_write_byte (thrift_protocol, (gint8)key_type);
    wsize += thrift_protocol_write_byte (thrift_protocol, (gint8)value_type);
    wsize += thrift_protocol_write_i32 (thrift_protocol, (gint32)size);
    return wsize;
}

guint32 thrift_protocol_write_map_end (ThriftProtocol * thrift_protocol)
{
    return 0;
}

guint32 thrift_protocol_write_list_begin (ThriftProtocol * thrift_protocol,
                                          const ThriftType element_type,
                                          const guint32 size)
{
    g_assert (THRIFT_IS_PROTOCOL (thrift_protocol));

    guint32 wsize = 0;
    wsize += thrift_protocol_write_byte (thrift_protocol, (gint8)element_type);
    wsize += thrift_protocol_write_i32 (thrift_protocol, (gint32)size);
    return wsize;
}

guint32 thrift_protocol_write_list_end (ThriftProtocol * thrift_protocol)
{
    return 0;
}

guint32 thrift_protocol_write_set_begin (ThriftProtocol * thrift_protocol,
                                         const ThriftType element_type,
                                         const guint32 size)
{
    g_assert (THRIFT_IS_PROTOCOL (thrift_protocol));

    return thrift_protocol_write_list_begin (thrift_protocol, element_type,
                                             size);
}

guint32 thrift_protocol_write_set_end (ThriftProtocol * thrift_protocol)
{
    return 0;
}

guint32 thrift_protocol_write_bool (ThriftProtocol * thrift_protocol,
                                    const gboolean value)
{
    g_assert (THRIFT_IS_PROTOCOL (thrift_protocol));

    guint8 tmp = value ? 1 : 0;
    return thrift_protocol_write_byte (thrift_protocol, tmp);
}

guint32 thrift_protocol_write_byte (ThriftProtocol * thrift_protocol,
                                    const gint8 byte)
{
    g_assert (THRIFT_IS_PROTOCOL (thrift_protocol));

    thrift_socket_send (thrift_protocol->thrift_socket, 
                        (const gpointer)&byte, 1);
    return 1;
}

guint32 thrift_protocol_write_i16 (ThriftProtocol * thrift_protocol,
                                   const gint16 i16)
{
    g_assert (THRIFT_IS_PROTOCOL (thrift_protocol));

    gint16 net = g_htons (i16);
    thrift_socket_send (thrift_protocol->thrift_socket, 
                        (const gpointer)&net, 2);
    return 2;
}

guint32 thrift_protocol_write_i32 (ThriftProtocol * thrift_protocol, 
                                   const gint32 i32)
{
    g_assert (THRIFT_IS_PROTOCOL (thrift_protocol));

    gint32 net = g_htonl (i32);
    thrift_socket_send (thrift_protocol->thrift_socket, 
                        (const gpointer)&net, 4);
    return 4;
}

guint32 thrift_protocol_write_i64 (ThriftProtocol * thrift_protocol,
                                   const gint64 i64)
{
    g_assert (THRIFT_IS_PROTOCOL (thrift_protocol));

    // TODO: 64-bit htonll???
    gint64 net = (gint64)g_htonl(i64);
    thrift_socket_send (thrift_protocol->thrift_socket, 
                        (const gpointer)&net, 8);
    return 8;
}

guint32 thrift_protocol_write_double (ThriftProtocol * thrift_protocol,
                                      const double dub)
{
    g_assert (sizeof (double) == sizeof (guint64));

    // TODO: 64-bit htonll???
    // TODO: is the "cast" enough?
    guint64 bits = (gint64)g_htonl(dub);
    return thrift_protocol_write_i64 (thrift_protocol, bits);
}

guint32 thrift_protocol_write_string (ThriftProtocol * thrift_protocol,
                                      const gchar * str)
{
    g_assert (THRIFT_IS_PROTOCOL (thrift_protocol));

    guint32 len = strlen (str);
    /* + 1 is for the null term char */
    return thrift_protocol_write_binary (thrift_protocol, str, len + 1);
}

guint32 thrift_protocol_write_binary (ThriftProtocol * thrift_protocol,
                                      const char * buf, const guint32 len)
{
    g_assert (THRIFT_IS_PROTOCOL (thrift_protocol));

    guint32 result = thrift_protocol_write_i32 (thrift_protocol, len);
    if (len > 0)
        thrift_socket_send (thrift_protocol->thrift_socket, 
                            (const gpointer)buf, len);
    return result + len;
}

gint32 thrift_protocol_read_message_begin (ThriftProtocol * thrift_protocol,
                                           gchar ** name, 
                                           ThriftMessageType * message_type,
                                           gint32 * seq_id)
{
    guint32 result = 0;
    gint32 sz;
    result += thrift_protocol_read_i32 (thrift_protocol, &sz);

    if (sz < 0)
    {
        // Check for correct version number
        gint32 version = sz & VERSION_MASK;
        /* TODO: error handling */
        if (version != VERSION_1)
            return -1;
        /* TODO: shouldn't this be xor with VERSION_MASK or something */
        *message_type = (ThriftMessageType)(sz & 0x000000ff);
        result += thrift_protocol_read_string (thrift_protocol, name);
        result += thrift_protocol_read_i32 (thrift_protocol, seq_id);
    }

    return result;
}

gint32 thrift_protocol_read_message_end (ThriftProtocol * thrift_protocol)
{
    return 0;
}

gint32 thrift_protocol_read_struct_begin (ThriftProtocol * thrift_protocol,
                                          gchar ** name)
{
    *name = NULL;
}

gint32 thrift_protocol_read_struct_end (ThriftProtocol * thrift_protocol)
{
    return 0;
}

gint32 thrift_protocol_read_field_begin (ThriftProtocol * thrift_protocol,
                                         gchar ** name,
                                         ThriftType * field_type,
                                         gint16 * field_id)
{
    guint32 result = 0;
    gint8 type;
    result += thrift_protocol_read_byte (thrift_protocol, &type);
    *field_type = (ThriftType)type;
    if (*field_type == T_STOP) 
    {
        *field_id = 0;
        return result;
    }
    result += thrift_protocol_read_i16 (thrift_protocol, field_id);
    return result;
}

gint32 thrift_protocol_read_field_end (ThriftProtocol * thrift_protocol)
{
    return 0;
}

gint32 thrift_protocol_read_map_begin (ThriftProtocol * thrift_protocol,
                                       ThriftType * key_type,
                                       ThriftType * value_type,
                                       guint32 * size)
{
    gint8 k, v;
    guint32 result = 0;
    gint32 sizei;
    result += thrift_protocol_read_byte (thrift_protocol, &k);
    *key_type = (ThriftType)k;
    result += thrift_protocol_read_byte (thrift_protocol, &v);
    *value_type = (ThriftType)v;
    result += thrift_protocol_read_i32 (thrift_protocol, &sizei);
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

gint32 thrift_protocol_read_map_end (ThriftProtocol * thrift_protocol)
{
    return 0;
}

gint32 thrift_protocol_read_list_begin (ThriftProtocol * thrift_protocol,
                                        ThriftType * element_type,
                                        guint32 * size)
{
    gint8 e;
    guint32 result = 0;
    gint32 sizei;
    result += thrift_protocol_read_byte (thrift_protocol, &e);
    *element_type = (ThriftType)e;
    result += thrift_protocol_read_i32 (thrift_protocol, &sizei);
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

gint32 thrift_protocol_read_list_end (ThriftProtocol * thrift_protocol)
{
    return 0;
}

gint32 thrift_protocol_read_set_begin (ThriftProtocol * thrift_protocol,
                                       ThriftType * element_type,
                                       guint32 * size)
{
    return thrift_protocol_read_list_begin (thrift_protocol, element_type, 
                                            size);
}

gint32 thrift_protocol_read_set_end (ThriftProtocol * thrift_protocol)
{
    return 0;
}

gint32 thrift_protocol_read_bool (ThriftProtocol * thrift_protocol,
                                  gboolean * value)
{
    gpointer b[1];
    thrift_socket_receive (thrift_protocol->thrift_socket, b, 1);
    *value = *(gint8*)b != 0;
    return 1;
}

gint32 thrift_protocol_read_byte (ThriftProtocol * thrift_protocol,
                                  gint8 * byte)
{
    gpointer b[1];
    thrift_socket_receive (thrift_protocol->thrift_socket, b, 1);
    *byte = *(gint8*)b;
    return 1;
}

gint32 thrift_protocol_read_i16 (ThriftProtocol * thrift_protocol,
                                 gint16 * i16)
{
    gpointer b[2];
    thrift_socket_receive (thrift_protocol->thrift_socket, b, 2);
    *i16 = *(gint16*)b;
    *i16 = g_ntohs(*i16);
    return 2;
}

gint32 thrift_protocol_read_i32 (ThriftProtocol * thrift_protocol,
                                 gint32 * i32)
{
    gpointer b[4];
    thrift_socket_receive (thrift_protocol->thrift_socket, b, 4);
    *i32 = *(gint32*)b;
    *i32 = g_ntohl(*i32);
    return 4;
}

gint32 thrift_protocol_read_i64 (ThriftProtocol * thrift_protocol,
                                 gint64 * i64)
{
    gpointer b[8];
    thrift_socket_receive (thrift_protocol->thrift_socket, b, 8);
    *i64 = *(gint64*)b;
    // TODO: 64-bit htonll???
    *i64 = g_ntohl(*i64);
    return 8;
}

gint32 thrift_protocol_read_double (ThriftProtocol * thrift_protocol,
                                    double * dub)
{
    // TODO: is the cast enough?
    gint32 result = thrift_protocol_read_i64 (thrift_protocol, (gint64*)dub);
    return result;
}

gint32 thrift_protocol_read_string (ThriftProtocol * thrift_protocol,
                                    gchar ** str)
{
    guint32 len;
    guint32 result = thrift_protocol_read_binary (thrift_protocol, str, &len);
    return result;
}

gint32 thrift_protocol_read_binary (ThriftProtocol * thrift_protocol,
                                    gchar ** str, guint32 * len)
{
    gint32 result = thrift_protocol_read_i32 (thrift_protocol, len);

    *str = g_new (gchar, *len);
    /* TODO: make sure we got the mem */
    gint ret = thrift_socket_receive (thrift_protocol->thrift_socket, *str, 
                                      *len);
    if (ret < 0 || ret != *len)
    {
        /* TODO: error handling */
        return -1;
    }

    return result + *len;
}

enum _ThriftProtocolProperties
{
    PROP_DUMMY,
    PROP_SOCKET,
};

void _thrift_protocol_set_property (GObject * object, guint property_id,
                                    const GValue * value, GParamSpec * pspec)
{
    ThriftProtocol * protocol = THRIFT_PROTOCOL (object);
    /* TODO: we could check that pspec is the type we want, not sure that's nec
     * TODO: proper error here */
    switch (property_id)
    {
        case PROP_SOCKET:
            protocol->thrift_socket = g_value_get_object (value);
            break;
    }
}

void _thrift_protocol_get_property (GObject * object, guint property_id,
                                    GValue * value, GParamSpec * pspec)
{
    ThriftProtocol * protocol = THRIFT_PROTOCOL (object);
    /* TODO: we could check that pspec is the type we want, not sure that's nec
     * TODO: proper error here */
    switch (property_id)
    {
        case PROP_SOCKET:
            g_value_set_object (value, protocol->thrift_socket);
            break;
    }
}

static void _thrift_protocol_instance_init (GTypeInstance *instance,
                                            gpointer g_class)
{
    ThriftProtocol * protocol = THRIFT_PROTOCOL (instance);
    protocol->thrift_socket = NULL;
}

// TODO: destroy, free hostname memory, close protocol, etc.


static void _thrift_protocol_class_init (gpointer klass,
                                         gpointer class_data)
{
    GObjectClass * gobject_class = G_OBJECT_CLASS (klass);
    GParamSpec * param_spec;

    gobject_class->set_property = _thrift_protocol_set_property;
    gobject_class->get_property = _thrift_protocol_get_property;

    param_spec = g_param_spec_object ("socket",
                                      "socket (construct)",
                                      "Set the socket of the protocol",
                                      THRIFT_SOCKET_TYPE,
                                      G_PARAM_CONSTRUCT_ONLY |
                                      G_PARAM_READWRITE);
    g_object_class_install_property (gobject_class, PROP_SOCKET, param_spec);
}

GType thrift_protocol_get_type (void)
{
    static GType type = 0;

    if (type == 0)
    {
        static const GTypeInfo type_info =
        {
            sizeof (ThriftProtocolClass),
            NULL, /* base_init */
            NULL, /* base_finalize */
            _thrift_protocol_class_init,
            NULL, /* class_finalize */
            NULL, /* class_data */
            sizeof (ThriftProtocol),
            0, /* n_preallocs */
            _thrift_protocol_instance_init
        };

        type = g_type_register_static (G_TYPE_OBJECT,
                                       "ThriftProtocolType",
                                       &type_info, 0);
    }

    return type;
}
