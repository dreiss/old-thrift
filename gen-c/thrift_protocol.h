/**
 * Thrift c
 *
 * THIS CLASS IS ABSTRACT, INSTANTIATE ONE OF ITS CHILDREN
 *
 */
#ifndef THRIFT_PROTOCOL_H
#define THRIFT_PROTOCOL_H

#include "thrift_transport.h"

#include <glib-object.h>

typedef enum _ThriftType ThriftType;
enum _ThriftType
{
    T_STOP       = 0,
    T_VOID       = 1,
    T_BOOL       = 2,
    T_BYTE       = 3,
    T_I08        = 3,
    T_I16        = 6,
    T_I32        = 8,
    T_U64        = 9,
    T_I64        = 10,
    T_DOUBLE     = 4,
    T_STRING     = 11,
    T_UTF7       = 11,
    T_STRUCT     = 12,
    T_MAP        = 13,
    T_SET        = 14,
    T_LIST       = 15,
    T_UTF8       = 16,
    T_UTF16      = 17
};

typedef enum _ThriftMessageType ThriftMessageType;
enum _ThriftMessageType
{
    T_CALL       = 1,
    T_REPLY      = 2,
    T_EXCEPTION  = 3
};

typedef struct _ThriftProtocol ThriftProtocol;
struct _ThriftProtocol
{
    GObject parent;

    /* private */
    ThriftTransport * transport;
};

typedef struct _ThriftProtocolClass ThriftProtocolClass;
struct _ThriftProtocolClass
{
    GObjectClass parent;

    gint32 (*write_message_begin) (ThriftProtocol * thrift_protocol,
                                   const gchar * name,
                                   const ThriftMessageType message_type,
                                   const gint32 seqid, GError ** error);
    gint32 (*write_message_end) (ThriftProtocol * thrift_protocol,
                                 GError ** error);
    gint32 (*write_struct_begin) (ThriftProtocol * thrift_protocol,
                                  const gchar * name, GError ** error);
    gint32 (*write_struct_end) (ThriftProtocol * thrift_protocol,
                                GError ** error);
    gint32 (*write_field_begin) (ThriftProtocol * thrift_protocol,
                                 const gchar * name,
                                 const ThriftType field_type,
                                 const gint16 field_id, GError ** error);
    gint32 (*write_field_end) (ThriftProtocol * thrift_protocol,
                               GError ** error);
    gint32 (*write_field_stop) (ThriftProtocol * thrift_protocol,
                                GError ** error);
    gint32 (*write_map_begin) (ThriftProtocol * thrift_protocol,
                               const ThriftType key_type,
                               const ThriftType value_type,
                               const guint32 size, GError ** error);
    gint32 (*write_map_end) (ThriftProtocol * thrift_protocol, GError ** error);
    gint32 (*write_list_begin) (ThriftProtocol * thrift_protocol,
                                const ThriftType element_type,
                                const guint32 size, GError ** error);
    gint32 (*write_list_end) (ThriftProtocol * thrift_protocol, GError ** error);
    gint32 (*write_set_begin) (ThriftProtocol * thrift_protocol,
                               const ThriftType element_type,
                               const guint32 size, GError ** error);
    gint32 (*write_set_end) (ThriftProtocol * thrift_protocol, GError ** error);
    gint32 (*write_bool) (ThriftProtocol * thrift_protocol,
                          const gboolean value, GError ** error);
    gint32 (*write_byte) (ThriftProtocol * thrift_protocol,
                          const gint8 value, GError ** error);
    gint32 (*write_i16) (ThriftProtocol * thrift_protocol,
                         const gint16 value, GError ** error);
    gint32 (*write_i32) (ThriftProtocol * thrift_protocol,
                         const gint32 value, GError ** error);
    gint32 (*write_i64) (ThriftProtocol * thrift_protocol,
                         const gint64 value, GError ** error);
    gint32 (*write_double) (ThriftProtocol * thrift_protocol,
                            const double value, GError ** error);
    gint32 (*write_string) (ThriftProtocol * thrift_protocol,
                            const gchar * str, GError ** error);
    gint32 (*write_binary) (ThriftProtocol * thrift_protocol,
                            const char * buf, const guint32 len, GError ** error);

    gint32 (*read_message_begin) (ThriftProtocol * thrift_protocol,
                                  gchar ** name,
                                  ThriftMessageType * message_type,
                                  gint32 * seqid, GError ** error);
    gint32 (*read_message_end) (ThriftProtocol * thrift_protocol,
                                GError ** error);
    gint32 (*read_struct_begin) (ThriftProtocol * thrift_protocol,
                                 gchar ** name, GError ** error);
    gint32 (*read_struct_end) (ThriftProtocol * thrift_protocol,
                               GError ** error);
    gint32 (*read_field_begin) (ThriftProtocol * thrift_protocol, gchar ** name,
                                ThriftType * field_type, gint16 * field_id,
                                GError ** error);
    gint32 (*read_field_end) (ThriftProtocol * thrift_protocol,
                              GError ** error);
    gint32 (*read_map_begin) (ThriftProtocol * thrift_protocol,
                              ThriftType * key_type, ThriftType * value_type,
                              guint32 * size, GError ** error);
    gint32 (*read_map_end) (ThriftProtocol * thrift_protocol, GError ** error);
    gint32 (*read_list_begin) (ThriftProtocol * thrift_protocol,
                               ThriftType * element_type, guint32 * size,
                               GError ** error);
    gint32 (*read_list_end) (ThriftProtocol * thrift_protocol, GError ** error);
    gint32 (*read_set_begin) (ThriftProtocol * thrift_protocol,
                              ThriftType * element_type, guint32 * size,
                              GError ** error);
    gint32 (*read_set_end) (ThriftProtocol * thrift_protocol, GError ** error);
    gint32 (*read_bool) (ThriftProtocol * thrift_protocol, gboolean * value,
                         GError ** error);
    gint32 (*read_byte) (ThriftProtocol * thrift_protocol, gint8 * value,
                         GError ** error);
    gint32 (*read_i16) (ThriftProtocol * thrift_protocol, gint16 * value,
                        GError ** error);
    gint32 (*read_i32) (ThriftProtocol * thrift_protocol, gint32 * value,
                        GError ** error);
    gint32 (*read_i64) (ThriftProtocol * thrift_protocol, gint64 * value,
                        GError ** error);
    gint32 (*read_double) (ThriftProtocol * thrift_protocol, double * value,
                           GError ** error);
    gint32 (*read_string) (ThriftProtocol * thrift_protocol, gchar ** str,
                           GError ** error);
    gint32 (*read_binary) (ThriftProtocol * thrift_protocol, gchar ** str,
                           guint32 * len, GError ** error);
};

GType thrift_protocol_get_type (void);

#define THRIFT_TYPE_PROTOCOL (thrift_protocol_get_type ())
#define THRIFT_PROTOCOL(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), THRIFT_TYPE_PROTOCOL, ThriftProtocol))
#define THRIFT_PROTOCOL_CLASS(c) (G_TYPE_CHECK_CLASS_CAST ((c), THRIFT_TYPE_PROTOCOL, ThriftProtocolClass))
#define THRIFT_IS_PROTOCOL(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), THRIFT_TYPE_PROTOCOL))
#define THRIFT_IS_PROTOCOL_CLASS(c) (G_TYPE_CHECK_CLASS_TYPE ((c), THRIFT_TYPE_PROTOCOL))
#define THRIFT_PROTOCOL_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), THRIFT_TYPE_PROTOCOL, ThriftProtocolClass))

gint32 thrift_protocol_write_message_begin (ThriftProtocol * thrift_protocol,
                                            const gchar * name,
                                            const ThriftMessageType message_type,
                                            const gint32 seqid,
                                            GError ** error);
gint32 thrift_protocol_write_message_end (ThriftProtocol * thrift_protocol,
                                          GError ** error);
gint32 thrift_protocol_write_struct_begin (ThriftProtocol * thrift_protocol,
                                           const gchar * name,
                                           GError ** error);
gint32 thrift_protocol_write_struct_end (ThriftProtocol * thrift_protocol,
                                         GError ** error);
gint32 thrift_protocol_write_field_begin (ThriftProtocol * thrift_protocol,
                                          const gchar * name,
                                          const ThriftType field_type,
                                          const gint16 field_id,
                                          GError ** error);
gint32 thrift_protocol_write_field_end (ThriftProtocol * thrift_protocol,
                                        GError ** error);
gint32 thrift_protocol_write_field_stop (ThriftProtocol * thrift_protocol,
                                         GError ** error);
gint32 thrift_protocol_write_map_begin (ThriftProtocol * thrift_protocol,
                                        const ThriftType key_type,
                                        const ThriftType value_type,
                                        const guint32 size, GError ** error);
gint32 thrift_protocol_write_map_end (ThriftProtocol * thrift_protocol,
                                      GError ** error);
gint32 thrift_protocol_write_list_begin (ThriftProtocol * thrift_protocol,
                                         const ThriftType element_type,
                                         const guint32 size, GError ** error);
gint32 thrift_protocol_write_list_end (ThriftProtocol * thrift_protocol,
                                       GError ** error);
gint32 thrift_protocol_write_set_begin (ThriftProtocol * thrift_protocol,
                                        const ThriftType element_type,
                                        const guint32 size, GError ** error);
gint32 thrift_protocol_write_set_end (ThriftProtocol * thrift_protocol,
                                      GError ** error);
gint32 thrift_protocol_write_bool (ThriftProtocol * thrift_protocol,
                                   const gboolean value, GError ** error);
gint32 thrift_protocol_write_byte (ThriftProtocol * thrift_protocol,
                                   const gint8 value, GError ** error);
gint32 thrift_protocol_write_i16 (ThriftProtocol * thrift_protocol,
                                  const gint16 value, GError ** error);
gint32 thrift_protocol_write_i32 (ThriftProtocol * thrift_protocol,
                                  const gint32 value, GError ** error);
gint32 thrift_protocol_write_i64 (ThriftProtocol * thrift_protocol,
                                  const gint64 value, GError ** error);
gint32 thrift_protocol_write_double (ThriftProtocol * thrift_protocol,
                                     const double value, GError ** error);
gint32 thrift_protocol_write_string (ThriftProtocol * thrift_protocol,
                                     const gchar * str, GError ** error);
gint32 thrift_protocol_write_binary (ThriftProtocol * thrift_protocol,
                                     const char * buf, const guint32 len,
                                     GError ** error);

gint32 thrift_protocol_read_message_begin (ThriftProtocol * thrift_protocol,
                                           gchar ** name,
                                           ThriftMessageType * message_type,
                                           gint32 * seqid, GError ** error);
gint32 thrift_protocol_read_message_end (ThriftProtocol * thrift_protocol,
                                         GError ** error);
gint32 thrift_protocol_read_struct_begin (ThriftProtocol * thrift_protocol,
                                          gchar ** name, GError ** error);
gint32 thrift_protocol_read_struct_end (ThriftProtocol * thrift_protocol,
                                        GError ** error);
gint32 thrift_protocol_read_field_begin (ThriftProtocol * thrift_protocol,
                                         gchar ** name,
                                         ThriftType * field_type,
                                         gint16 * field_id, GError ** error);
gint32 thrift_protocol_read_field_end (ThriftProtocol * thrift_protocol,
                                       GError ** error);
gint32 thrift_protocol_read_map_begin (ThriftProtocol * thrift_protocol,
                                       ThriftType * key_type,
                                       ThriftType * value_type,
                                       guint32 * size, GError ** error);
gint32 thrift_protocol_read_map_end (ThriftProtocol * thrift_protocol,
                                     GError ** error);
gint32 thrift_protocol_read_list_begin (ThriftProtocol * thrift_protocol,
                                        ThriftType * element_type,
                                        guint32 * size, GError ** error);
gint32 thrift_protocol_read_list_end (ThriftProtocol * thrift_protocol,
                                      GError ** error);
gint32 thrift_protocol_read_set_begin (ThriftProtocol * thrift_protocol,
                                       ThriftType * element_type,
                                       guint32 * size, GError ** error);
gint32 thrift_protocol_read_set_end (ThriftProtocol * thrift_protocol,
                                     GError ** error);
gint32 thrift_protocol_read_bool (ThriftProtocol * thrift_protocol,
                                  gboolean * value, GError ** error);
gint32 thrift_protocol_read_byte (ThriftProtocol * thrift_protocol,
                                  gint8 * value, GError ** error);
gint32 thrift_protocol_read_i16 (ThriftProtocol * thrift_protocol,
                                 gint16 * value, GError ** error);
gint32 thrift_protocol_read_i32 (ThriftProtocol * thrift_protocol,
                                 gint32 * value, GError ** error);
gint32 thrift_protocol_read_i64 (ThriftProtocol * thrift_protocol,
                                 gint64 * value, GError ** error);
gint32 thrift_protocol_read_double (ThriftProtocol * thrift_protocol,
                                    double * value, GError ** error);
gint32 thrift_protocol_read_string (ThriftProtocol * thrift_protocol,
                                    gchar ** str, GError ** error);
gint32 thrift_protocol_read_binary (ThriftProtocol * thrift_protocol,
                                    gchar ** str, guint32 * len,
                                    GError ** error);

gint32 thrift_protocol_skip (ThriftProtocol * thrift_protocol, ThriftType type,
                             GError ** error);

#endif /* THRIFT_PROTOCOL_H */
