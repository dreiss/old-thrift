/**
 * Thrift c 
 *
 */
#ifndef THRIFT_BINARY_PROTOCOL_H
#define THRIFT_BINARY_PROTOCOL_H

#include "thrift_protocol.h"

#include <glib-object.h>

typedef struct _ThriftBinaryProtocol ThriftBinaryProtocol;
struct _ThriftBinaryProtocol
{ 
    ThriftProtocol parent; 

    /* private */
    ThriftTransport * transport;
}; 

typedef struct _ThriftBinaryProtocolClass ThriftBinaryProtocolClass;
struct _ThriftBinaryProtocolClass
{ 
    ThriftProtocolClass parent; 
}; 

GType thrift_binary_protocol_get_type (void);

#define THRIFT_TYPE_BINARY_PROTOCOL (thrift_binary_protocol_get_type ())
#define THRIFT_BINARY_PROTOCOL(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), THRIFT_TYPE_BINARY_PROTOCOL, ThriftBinaryProtocol))
#define THRIFT_BINARY_PROTOCOL_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), THRIFT_TYPE_BINARY_PROTOCOL, ThriftBinaryProtocolClass))
#define THRIFT_IS_BINARY_PROTOCOL(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), THRIFT_TYPE_BINARY_PROTOCOL))
#define THRIFT_IS_BINARY_PROTOCOL_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), THRIFT_TYPE_BINARY_PROTOCOL))
#define THRIFT_BINARY_PROTOCOL_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), THRIFT_TYPE_BINARY_PROTOCOL, ThriftBinaryProtocolClass))

#define VERSION_1 0x80010000
#define VERSION_MASK 0xffff0000

typedef enum
{
    THRIFT_BINARY_PROTOCOL_ERROR_WRONG_VERSION,
    THRIFT_BINARY_PROTOCOL_ERROR_NEGATIVE_SIZE
} ThriftBinaryProtocolError;

GQuark thrift_binary_protocol_error_quark (void);
#define THRIFT_BINARY_PROTOCOL_ERROR (thrift_binary_protocol_error_quark ())

#endif /* THRIFT_BINARY_PROTOCOL_H */
