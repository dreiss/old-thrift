/**
 * Thrift c 
 *
 */

#ifndef THRIFT_CLIENT_H
#define THRIFT_CLIENT_H

#include "protocol/thrift_protocol.h"

#include <glib-object.h>

typedef struct _ThriftClient ThriftClient;
struct _ThriftClient
{
  GObject parent;

  /* private */
  ThriftProtocol * protocol;
};
typedef struct _ThriftClientClass ThriftClientClass;
struct _ThriftClientClass
{
  GObjectClass parent;
};

GType thrift_client_get_type (void);

#define THRIFT_TYPE_CLIENT (thrift_client_get_type ())
#define THRIFT_CLIENT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), THRIFT_TYPE_CLIENT, ThriftClient))
#define THRIFT_CLIENT_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), THRIFT_TYPE_CLIENT, ThriftClientClass))
#define THRIFT_IS_CLIENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), THRIFT_TYPE_CLIENT))
#define THRIFT_IS_CLIENT_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), THRIFT_TYPE_CLIENT))
#define THRIFT_CLIENT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), THRIFT_TYPE_CLIENT, ThriftClientClass))

#endif /* THRIFT_CLIENT_H */
