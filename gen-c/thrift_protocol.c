/**
 * Thrift c
 *
 */

/** TODO:
 * figure out how to make this class glib-object abstract
 */

#include "thrift_protocol.h"

#include <string.h>

gint32 thrift_protocol_write_message_begin (ThriftProtocol * thrift_protocol,
                                            const gchar * name, 
                                            const ThriftMessageType message_type, 
                                            const gint32 seqid)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_message_begin (thrift_protocol, name, message_type, seqid);
}

gint32 thrift_protocol_write_message_end (ThriftProtocol * thrift_protocol)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_message_end (thrift_protocol);
}

gint32 thrift_protocol_write_struct_begin (ThriftProtocol * thrift_protocol,
                                           const gchar * name)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_struct_begin (thrift_protocol, name);
}

gint32 thrift_protocol_write_struct_end (ThriftProtocol * thrift_protocol)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_struct_end (thrift_protocol);
}

gint32 thrift_protocol_write_field_begin (ThriftProtocol * thrift_protocol,
                                          const gchar * name,
                                          const ThriftType field_type,
                                          const gint16 field_id)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_field_begin (thrift_protocol, name, field_type, field_id);
}

gint32 thrift_protocol_write_field_end (ThriftProtocol * thrift_protocol)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_field_end (thrift_protocol);
}

gint32 thrift_protocol_write_field_stop (ThriftProtocol * thrift_protocol)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_field_stop (thrift_protocol);
}

gint32 thrift_protocol_write_map_begin (ThriftProtocol * thrift_protocol,
                                        const ThriftType key_type,
                                        const ThriftType value_type,
                                        const guint32 size)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_map_begin (thrift_protocol, key_type, value_type, size);
}

gint32 thrift_protocol_write_map_end (ThriftProtocol * thrift_protocol)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_map_end (thrift_protocol);
}

gint32 thrift_protocol_write_list_begin (ThriftProtocol * thrift_protocol,
                                         const ThriftType element_type,
                                         const guint32 size)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_list_begin (thrift_protocol, element_type, size);
}

gint32 thrift_protocol_write_list_end (ThriftProtocol * thrift_protocol)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_list_end (thrift_protocol);
}

gint32 thrift_protocol_write_set_begin (ThriftProtocol * thrift_protocol,
                                        const ThriftType element_type,
                                        const guint32 size)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_set_begin (thrift_protocol, element_type, size);
}

gint32 thrift_protocol_write_set_end (ThriftProtocol * thrift_protocol)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_list_end (thrift_protocol);
}

gint32 thrift_protocol_write_bool (ThriftProtocol * thrift_protocol,
                                   const gboolean value)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_bool (thrift_protocol, value);
}

gint32 thrift_protocol_write_byte (ThriftProtocol * thrift_protocol,
                                   const gint8 value)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_byte (thrift_protocol, value);
}

gint32 thrift_protocol_write_i16 (ThriftProtocol * thrift_protocol,
                                  const gint16 value)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_i16 (thrift_protocol, value);
}

gint32 thrift_protocol_write_i32 (ThriftProtocol * thrift_protocol, 
                                  const gint32 value)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_i32 (thrift_protocol, value);
}

gint32 thrift_protocol_write_i64 (ThriftProtocol * thrift_protocol,
                                  const gint64 value)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_i64 (thrift_protocol, value);
}

gint32 thrift_protocol_write_double (ThriftProtocol * thrift_protocol,
                                     const double value)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_double (thrift_protocol, value);
}

gint32 thrift_protocol_write_string (ThriftProtocol * thrift_protocol,
                                     const gchar * str)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_string (thrift_protocol, str);
}

gint32 thrift_protocol_write_binary (ThriftProtocol * thrift_protocol,
                                     const char * buf, const guint32 len)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        write_binary (thrift_protocol, buf, len);
}

gint32 thrift_protocol_read_message_begin (ThriftProtocol * thrift_protocol,
                                           gchar ** name, 
                                           ThriftMessageType * message_type,
                                           gint32 * seqid)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        read_message_begin (thrift_protocol, name, message_type, seqid);
}

gint32 thrift_protocol_read_message_end (ThriftProtocol * thrift_protocol)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        read_message_end (thrift_protocol);
}

gint32 thrift_protocol_read_struct_begin (ThriftProtocol * thrift_protocol,
                                          gchar ** name)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        read_struct_begin (thrift_protocol, name);
}

gint32 thrift_protocol_read_struct_end (ThriftProtocol * thrift_protocol)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        read_struct_end (thrift_protocol);
}

gint32 thrift_protocol_read_field_begin (ThriftProtocol * thrift_protocol,
                                         gchar ** name,
                                         ThriftType * field_type,
                                         gint16 * field_id)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        read_field_begin (thrift_protocol, name, field_type, field_id);
}

gint32 thrift_protocol_read_field_end (ThriftProtocol * thrift_protocol)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        read_field_end (thrift_protocol);
}

gint32 thrift_protocol_read_map_begin (ThriftProtocol * thrift_protocol,
                                       ThriftType * key_type,
                                       ThriftType * value_type,
                                       guint32 * size)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        read_map_begin (thrift_protocol, key_type, value_type, size);
}

gint32 thrift_protocol_read_map_end (ThriftProtocol * thrift_protocol)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        read_map_end (thrift_protocol);
}

gint32 thrift_protocol_read_list_begin (ThriftProtocol * thrift_protocol,
                                        ThriftType * element_type,
                                        guint32 * size)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        read_list_begin (thrift_protocol, element_type, size);
}

gint32 thrift_protocol_read_list_end (ThriftProtocol * thrift_protocol)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        read_list_end (thrift_protocol);
}

gint32 thrift_protocol_read_set_begin (ThriftProtocol * thrift_protocol,
                                       ThriftType * element_type,
                                       guint32 * size)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        read_set_begin (thrift_protocol, element_type, size);
}

gint32 thrift_protocol_read_set_end (ThriftProtocol * thrift_protocol)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        read_set_end (thrift_protocol);
}

gint32 thrift_protocol_read_bool (ThriftProtocol * thrift_protocol,
                                  gboolean * value)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        read_bool (thrift_protocol, value);
}

gint32 thrift_protocol_read_byte (ThriftProtocol * thrift_protocol,
                                  gint8 * value)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        read_byte (thrift_protocol, value);
}

gint32 thrift_protocol_read_i16 (ThriftProtocol * thrift_protocol,
                                 gint16 * value)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        read_i16 (thrift_protocol, value);
}

gint32 thrift_protocol_read_i32 (ThriftProtocol * thrift_protocol,
                                 gint32 * value)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        read_i32 (thrift_protocol, value);
}

gint32 thrift_protocol_read_i64 (ThriftProtocol * thrift_protocol,
                                 gint64 * value)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        read_i64 (thrift_protocol, value);
}

gint32 thrift_protocol_read_double (ThriftProtocol * thrift_protocol,
                                    double * value)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        read_double (thrift_protocol, value);
}

gint32 thrift_protocol_read_string (ThriftProtocol * thrift_protocol,
                                    gchar ** str)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        read_string (thrift_protocol, str);
}

gint32 thrift_protocol_read_binary (ThriftProtocol * thrift_protocol,
                                    gchar ** buf, guint32 * len)
{
    return THRIFT_PROTOCOL_GET_CLASS (thrift_protocol)->
        read_binary (thrift_protocol, buf, len);
}

/**
 * Method to arbitrarily skip over data.
 */
gint32 thrift_protocol_skip (ThriftProtocol * thrift_protocol, ThriftType type)
{
    switch (type) 
    {
        case T_BOOL:
            {
                gboolean boolv;
                return thrift_protocol_read_bool (thrift_protocol, &boolv);
            }
        case T_BYTE:
            {
                gint8 bytev;
                return thrift_protocol_read_byte (thrift_protocol, &bytev);
            }
        case T_I16:
            {
                gint16 i16;
                return thrift_protocol_read_i16 (thrift_protocol, &i16);
            }
        case T_I32:
            {
                gint32 i32;
                return thrift_protocol_read_i32 (thrift_protocol, &i32);
            }
        case T_I64:
            {
                gint64 i64;
                return thrift_protocol_read_i64 (thrift_protocol, &i64);
            }
        case T_DOUBLE:
            {
                double dub;
                return thrift_protocol_read_double (thrift_protocol, &dub);
            }
        case T_STRING:
            {
                gchar * str;
                guint32 len;
                gint32 ret = thrift_protocol_read_binary (thrift_protocol, &str, 
                                                          &len);
                g_free (str);
                return ret;
            }
        case T_STRUCT:
            {
                guint32 result = 0;
                gchar * name;
                gint16 fid;
                ThriftType ftype;
                result += thrift_protocol_read_struct_begin (thrift_protocol, &name);
                while (1) 
                {
                    result += thrift_protocol_read_field_begin (thrift_protocol, 
                                                                &name, &ftype, &fid);
                    if (ftype == T_STOP)
                        break;
                    result += thrift_protocol_skip (thrift_protocol, ftype);
                    result += thrift_protocol_read_field_end (thrift_protocol);
                }
                result += thrift_protocol_read_struct_end (thrift_protocol);
                return result;
            }
        case T_MAP:
            {
                guint32 result = 0;
                ThriftType key_type;
                ThriftType val_type;
                guint32 i, size;
                result += thrift_protocol_read_map_begin (thrift_protocol, &key_type,
                                                          &val_type, &size);
                for (i = 0; i < size; i++) {
                    result += thrift_protocol_skip (thrift_protocol, key_type);
                    result += thrift_protocol_skip (thrift_protocol, val_type);
                }
                result += thrift_protocol_read_map_end (thrift_protocol);
                return result;
            }
        case T_SET:
            {
                guint32 result = 0;
                ThriftType elem_type;
                guint32 i, size;
                result += thrift_protocol_read_set_begin (thrift_protocol, &elem_type, 
                                                          &size);
                for (i = 0; i < size; i++) {
                    result += thrift_protocol_skip (thrift_protocol, elem_type);
                }
                result += thrift_protocol_read_set_end (thrift_protocol);
                return result;
            }
        case T_LIST:
            {
                guint32 result = 0;
                ThriftType elem_type;
                guint32 i, size;
                result += thrift_protocol_read_list_begin (thrift_protocol, 
                                                           &elem_type, &size);
                for (i = 0; i < size; i++) {
                    result += thrift_protocol_skip (thrift_protocol, elem_type);
                }
                result += thrift_protocol_read_list_end (thrift_protocol);
                return result;
            }
        default:
            return 0;
    }
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

static void _thrift_protocol_instance_init (ThriftProtocol * protocol)
{
    protocol->thrift_socket = NULL;
}

static void _thrift_protocol_class_init (ThriftProtocolClass * klass)
{
    GObjectClass * gobject_class = G_OBJECT_CLASS (klass);
    GParamSpec * param_spec;

    gobject_class->set_property = _thrift_protocol_set_property;
    gobject_class->get_property = _thrift_protocol_get_property;

    param_spec = g_param_spec_object ("socket",
                                      "socket (construct)",
                                      "Set the socket of the protocol",
                                      THRIFT_TYPE_SOCKET,
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
            (GClassInitFunc)_thrift_protocol_class_init,
            NULL, /* class_finalize */
            NULL, /* class_data */
            sizeof (ThriftProtocol),
            0, /* n_preallocs */
            (GInstanceInitFunc)_thrift_protocol_instance_init,
            NULL, /* value_table */
        };

        type = g_type_register_static (G_TYPE_OBJECT,
                                       "ThriftProtocolType",
                                       &type_info, 0);
    }

    return type;
}
