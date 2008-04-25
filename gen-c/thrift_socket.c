/**
 * Thrift c
 *
 */

#include "thrift_socket.h"

#include <netdb.h>
#include <string.h>

gboolean thrift_socket_connect (ThriftSocket * thrift_socket)
{
    g_assert (THRIFT_IS_SOCKET (thrift_socket));
    g_assert (thrift_socket->socket == 0);

    struct hostent * hp;
    struct sockaddr_in pin;

    /* go find out about the desired host machine */
    if ((hp = gethostbyname (thrift_socket->hostname)) == 0)
    {
        perror ("gethostbyname");
        return 0;
    }

    /* fill in the socket structure with host information */
    memset (&pin, 0, sizeof (pin));
    pin.sin_family = AF_INET;
    pin.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
    pin.sin_port = htons (thrift_socket->port);

    /* grab an Internet domain socket */
    if ((thrift_socket->socket = socket (AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror ("socket");
        return 0;
    }

    /* connect to PORT on HOST */
    if (connect (thrift_socket->socket, (struct sockaddr *)&pin, sizeof (pin)) ==
        -1)
    {
        perror ("connect");
        return 0;
    }

    return 1;
}

gint thrift_socket_send (ThriftSocket * socket, const gpointer buf, guint len)
{
    guint ret;

    g_assert (THRIFT_IS_SOCKET (socket));

    /* send a message to the server PORT on machine HOST */
    ret = send (socket->socket, buf, len, 0);
    if (ret == -1)
        perror ("send");

    return ret;
}

gint thrift_socket_receive (ThriftSocket * socket, gpointer buf, guint len)
{
    g_assert (THRIFT_IS_SOCKET (socket));

    guint ret;

    /* wait for a message to come back from the server */
    ret = recv (socket->socket, buf, len, 0); 
    if (ret == -1)
        perror ("recv");
    return ret;
}

enum _ThriftSocketProperties
{
    PROP_DUMMY,
    PROP_HOSTNAME,
    PROP_PORT
};

void _thrift_socket_set_property (GObject * object, guint property_id,
                                  const GValue * value, GParamSpec * pspec)
{
    ThriftSocket * socket = THRIFT_SOCKET (object);
    /* TODO: we could check that pspec is the type we want, not sure that's nec
     * TODO: proper error here */
    switch (property_id)
    {
        case PROP_HOSTNAME:
            socket->hostname = g_strdup (g_value_get_string (value));
            break;
        case PROP_PORT:
            socket->port = g_value_get_uint (value);
            break;
    }
}

void _thrift_socket_get_property (GObject * object, guint property_id,
                                  GValue * value, GParamSpec * pspec)
{
    ThriftSocket * socket = THRIFT_SOCKET (object);
    /* TODO: we could check that pspec is the type we want, not sure that's nec
     * TODO: proper error here */
    switch (property_id)
    {
        case PROP_HOSTNAME:
            g_value_set_string (value, socket->hostname);
            break;
        case PROP_PORT:
            g_value_set_uint (value, socket->port);
            break;
    }
}

static void _thrift_socket_instance_init (ThriftSocket * socket)
{
    socket->socket = 0;
}

// TODO: destroy, free hostname memory, close socket, etc.


static void _thrift_socket_class_init (ThriftSocketClass * klass)
{
    GObjectClass * gobject_class = G_OBJECT_CLASS (klass);
    GParamSpec * param_spec;

    gobject_class->set_property = _thrift_socket_set_property;
    gobject_class->get_property = _thrift_socket_get_property;

    param_spec = g_param_spec_string ("hostname",
                                      "hostname (construct)",
                                      "Set the hostname of the remote host",
                                      "localhost" /* default value */,
                                      G_PARAM_CONSTRUCT_ONLY |
                                      G_PARAM_READWRITE);
    g_object_class_install_property (gobject_class, PROP_HOSTNAME, param_spec);

    param_spec = g_param_spec_uint ("port",
                                    "port (construct)",
                                    "Set the port of the remote host",
                                    0 /* min */,
                                    64000 /* max, TODO: actual max port value */,
                                    1024 /* default value */,
                                    G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
    g_object_class_install_property (gobject_class, PROP_PORT, param_spec);
}

GType thrift_socket_get_type (void)
{
    static GType type = 0;

    if (type == 0)
    {
        static const GTypeInfo type_info =
        {
            sizeof (ThriftSocketClass),
            NULL, /* base_init */
            NULL, /* base_finalize */
            (GClassInitFunc)_thrift_socket_class_init,
            NULL, /* class_finalize */
            NULL, /* class_data */
            sizeof (ThriftSocket),
            0, /* n_preallocs */
            (GInstanceInitFunc)_thrift_socket_instance_init,
            NULL, /* value_table */
        };

        type = g_type_register_static (G_TYPE_OBJECT,
                                       "ThriftSocketType",
                                       &type_info, 0);
    }

    return type;
}
