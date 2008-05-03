/**
 * Thrift c
 *
 */
#ifndef THRIFT_HASH_H
#define THRIFT_HASH_H

#include <glib.h>

gboolean thrift_gboolean_equal (gconstpointer v1, gconstpointer v2);
guint thrift_gboolean_hash (gconstpointer v);

gboolean thrift_gint8_equal (gconstpointer v1, gconstpointer v2);
guint thrift_gint8_hash (gconstpointer v);

gboolean thrift_gint16_equal (gconstpointer v1, gconstpointer v2);
guint thrift_gint16_hash (gconstpointer v);

gboolean thrift_gint32_equal (gconstpointer v1, gconstpointer v2);
guint thrift_gint32_hash (gconstpointer v);

gboolean thrift_gdouble_equal (gconstpointer v1, gconstpointer v2);
guint thrift_gdouble_hash (gconstpointer v);

#endif /* THRIFT_HASH_H */
