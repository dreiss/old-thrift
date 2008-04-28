/**
 * Thrift c
 *
 */

#include "thrift.h"
#include "thrift_client.h"

#include <netdb.h>
#include <string.h>

enum _ThriftClientProperties
{
    PROP_DUMMY,
    PROP_PROTOCOL
};

void _thrift_client_set_property (GObject * object, guint property_id,
                                  const GValue * value, GParamSpec * pspec)
{
    ThriftClient * client = THRIFT_CLIENT (object);

    THRIFT_UNUSED_VAR (pspec);

    /* TODO: we could check that pspec is the type we want, not sure that's nec
     * TODO: proper error here */
    switch (property_id)
    {
        case PROP_PROTOCOL:
            client->protocol = g_value_get_object (value);
            break;
    }
}

void _thrift_client_get_property (GObject * object, guint property_id,
                                  GValue * value, GParamSpec * pspec)
{
    ThriftClient * client = THRIFT_CLIENT (object);

    THRIFT_UNUSED_VAR (pspec);

    /* TODO: we could check that pspec is the type we want, not sure that's nec
     * TODO: proper error here */
    switch (property_id)
    {
        case PROP_PROTOCOL:
            g_value_set_object (value, client->protocol);
            break;
    }
}

static void _thrift_client_instance_init (ThriftClient * client)
{
    client->protocol = NULL;
}

static void _thrift_client_class_init (ThriftClientClass * klass)
{
    GObjectClass * gobject_class = G_OBJECT_CLASS (klass);
    GParamSpec * param_spec;

    gobject_class->set_property = _thrift_client_set_property;
    gobject_class->get_property = _thrift_client_get_property;

    param_spec = g_param_spec_object ("protocol",
                                      "protocol (construct)",
                                      "Set the protocol of the client",
                                      THRIFT_TYPE_PROTOCOL,
                                      G_PARAM_CONSTRUCT_ONLY |
                                      G_PARAM_READWRITE);
    g_object_class_install_property (gobject_class, PROP_PROTOCOL, param_spec);
}

GType thrift_client_get_type (void)
{
    static GType type = 0;

    if (type == 0)
    {
        static const GTypeInfo type_info =
        {
            sizeof (ThriftClientClass),
            NULL, /* base_init */
            NULL, /* base_finalize */
            (GClassInitFunc)_thrift_client_class_init,
            NULL, /* class_finalize */
            NULL, /* class_data */
            sizeof (ThriftClient),
            0, /* n_preallocs */
            (GInstanceInitFunc)_thrift_client_instance_init,
            NULL, /* value_table */
        };

        type = g_type_register_static (G_TYPE_OBJECT,
                                       "ThriftClientType",
                                       &type_info, 0);
    }

    return type;
}
