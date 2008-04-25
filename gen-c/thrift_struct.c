/**
 * Thrift c
 *
 */

#include "thrift_struct.h"

gint32 thrift_struct_write (ThriftStruct * object, ThriftProtocol * thrift_protocol)
{
    return THRIFT_STRUCT_GET_CLASS (object)->write (object, thrift_protocol);
}

static void _thrift_struct_instance_init (ThriftStruct * object)
{
    object->bucket = "";
    object->key = "";
    object->value = "";
}

static void _thrift_struct_class_init (ThriftStructClass * klass)
{
    ThriftStructClass * thrift_struct_klass = THRIFT_STRUCT_CLASS (klass);

    thrift_struct_klass->write = NULL;
}

GType thrift_struct_get_type (void)
{
    static GType type = 0;

    if (type == 0) 
    {
        static const GTypeInfo type_info = 
        {
            sizeof (ThriftStructClass),
            NULL, /* base_init */
            NULL, /* base_finalize */
            (GClassInitFunc)_thrift_struct_class_init,
            NULL, /* class_finalize */
            NULL, /* class_data */
            sizeof (ThriftStruct),
            0, /* n_preallocs */
            (GInstanceInitFunc)_thrift_struct_instance_init,
            NULL, /* value_table */
        };

        type = g_type_register_static (G_TYPE_OBJECT, 
            "ThriftStructType", /* type name as string */
            &type_info, 0);
    }

    return type;
}
