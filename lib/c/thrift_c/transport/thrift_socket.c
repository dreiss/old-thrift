/**
 * Thrift c
 *
 */

#include "thrift.h"
#include "thrift_socket.h"

#include <netdb.h>
#include <string.h>
#include <unistd.h>

/** TODO:
 * - support for read timeouts
 */

gboolean _thrift_socket_is_open (ThriftTransport * transport)
{
    ThriftSocket * socket = THRIFT_SOCKET (transport);
    return socket->sd != 0;
}

gboolean _thrift_socket_open (ThriftTransport * transport,
                              GError ** error)
{
    ThriftSocket * tsocket = THRIFT_SOCKET (transport);
    g_assert (tsocket->sd == 0);

    struct hostent * hp;
    struct sockaddr_in pin;

    /* go find out about the desired host machine */
    if ((hp = gethostbyname (tsocket->hostname)) == 0)
    {
        /* TODO: pass through h_error info */
        g_set_error (error, THRIFT_SOCKET_ERROR, THRIFT_SOCKET_ERROR_HOST, 
                     "failed to lookup host: %s:%d", 
                     tsocket->hostname, tsocket->port);
        return 0;
    }

    /* fill in the socket structure with host information */
    memset (&pin, 0, sizeof (pin));
    pin.sin_family = AF_INET;
    pin.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
    pin.sin_port = htons (tsocket->port);

    /* grab an Internet domain socket */
    if ((tsocket->sd = socket (AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        /* TODO: pass through error info */
        g_set_error (error, THRIFT_SOCKET_ERROR, THRIFT_SOCKET_ERROR_SOCKET, 
                     "failed to create socket for host: %s:%d", 
                     tsocket->hostname, tsocket->port);
        return 0;
    }

    /* connect to PORT on HOST */
    if (connect (tsocket->sd, (struct sockaddr *)&pin, sizeof (pin)) ==
        -1)
    {
        /* TODO: pass through error info */
        g_set_error (error, THRIFT_SOCKET_ERROR, THRIFT_SOCKET_ERROR_CONNECT, 
                     "failed to connect to host: %s:%d", 
                     tsocket->hostname, tsocket->port);
        return 0;
    }

    return 1;
}

gboolean _thrift_socket_close (ThriftTransport * transport,
                               GError ** error)
{
    ThriftSocket * socket = THRIFT_SOCKET (transport);

    THRIFT_UNUSED_VAR (error);

    /* TODO: error handling */
    if (!close (socket->sd))
    {
        g_set_error (error, 
    }
    socket->sd = 0;

    return 1;
}

gint32 _thrift_socket_read (ThriftTransport * transport, gpointer buf,
                            guint len, GError ** error)
{
    ThriftSocket * socket = THRIFT_SOCKET (transport);

    /* TODO: look at all the stuff in TSocket.cpp */
    gint ret;
    guint got = 0;
    while (got < len)
    {
        ret = recv (socket->sd, buf, len, 0); 
        if (ret < 0)
        {
            /* TODO: pass on error info */
            g_set_error (error, THRIFT_SOCKET_ERROR, 
                         THRIFT_SOCKET_ERROR_RECEIVE, 
                         "failed to send %d bytes", len);
            return -1;
        }
        got += ret;
    }

    return got;
}

gboolean _thrift_socket_read_end (ThriftTransport * transport, 
                                  GError ** error)
{
    THRIFT_UNUSED_VAR (transport);
    THRIFT_UNUSED_VAR (error);
    return 1;
}

gint32 _thrift_socket_write (ThriftTransport * transport,
                             gconstpointer buf, const guint len,
                             GError ** error)
{
    ThriftSocket * socket = THRIFT_SOCKET (transport);
    g_assert (socket->sd != 0);

    /* TODO: take a look at the flags stuff in TSocket.cpp */
    gint ret;
    guint sent = 0;
    while (sent < len)
    {
        ret = send (socket->sd, buf + sent, len - sent, 0);
        if (ret < 0)
        {
            /* TODO: pass on error info */
            g_set_error (error, THRIFT_SOCKET_ERROR, THRIFT_SOCKET_ERROR_SEND, 
                         "failed to send %d bytes", len);
            return -1;
        }
        sent += ret;
    }

    return sent;
}

gboolean _thrift_socket_write_end (ThriftTransport * transport, 
                                   GError ** error)
{
    THRIFT_UNUSED_VAR (transport);
    THRIFT_UNUSED_VAR (error);
    return 1;
}

gboolean _thrift_socket_flush (ThriftTransport * transport, GError ** error)
{
    THRIFT_UNUSED_VAR (transport);
    THRIFT_UNUSED_VAR (error);
    return 1;
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

    THRIFT_UNUSED_VAR (pspec);

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

    THRIFT_UNUSED_VAR (pspec);

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

/* TODO: this might should be a constrctor... */
static void _thrift_socket_instance_init (ThriftSocket * socket)
{
    socket->sd = 0;
}

static void
_thrift_socket_finalize (GObject * object)
{
    ThriftSocket * socket = THRIFT_SOCKET (object);

    if (socket->hostname != NULL)
        g_free (socket->hostname);
    socket->hostname = NULL;

    if (socket->sd != 0)
        close (socket->sd);
    socket->sd = 0;

    /* TODO: do we need to chain up here? */
}

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

    ThriftTransportClass * thrift_transport_class = 
        THRIFT_TRANSPORT_CLASS(klass);

    gobject_class->finalize = _thrift_socket_finalize;
    thrift_transport_class->is_open = _thrift_socket_is_open;
    thrift_transport_class->open = _thrift_socket_open;
    thrift_transport_class->close = _thrift_socket_close;
    thrift_transport_class->read = _thrift_socket_read;
    thrift_transport_class->read_end = _thrift_socket_read_end;
    thrift_transport_class->write = _thrift_socket_write;
    thrift_transport_class->write_end = _thrift_socket_write_end;
    thrift_transport_class->flush = _thrift_socket_flush;
}

GQuark
thrift_socket_error_quark (void)
{
    return g_quark_from_static_string ("thrift-socket-error-quark");
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

        type = g_type_register_static (THRIFT_TYPE_TRANSPORT,
                                       "ThriftSocketType",
                                       &type_info, 0);
    }

    return type;
}
