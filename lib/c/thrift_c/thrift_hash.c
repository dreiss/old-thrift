/**
 * Thrift c
 *
 */

#include "thrift_hash.h"

gboolean
thrift_gboolean_equal (gconstpointer v1,
                       gconstpointer v2)
{
    return *((const gboolean*) v1) == *((const gboolean*) v2);
}

guint
thrift_gboolean_hash (gconstpointer v)
{
    return *(const gboolean*) v;
}

gboolean
thrift_gint8_equal (gconstpointer v1,
                    gconstpointer v2)
{
    return *((const gint8*) v1) == *((const gint8*) v2);
}

guint
thrift_gint8_hash (gconstpointer v)
{
    return *(const gint8*) v;
}

gboolean
thrift_gint16_equal (gconstpointer v1,
                     gconstpointer v2)
{
    return *((const gint16*) v1) == *((const gint16*) v2);
}

guint
thrift_gint16_hash (gconstpointer v)
{
    return *(const gint16*) v;
}

gboolean
thrift_gint32_equal (gconstpointer v1,
                     gconstpointer v2)
{
    return *((const gint32*) v1) == *((const gint32*) v2);
}

guint
thrift_gint32_hash (gconstpointer v)
{
    return *(const gint32*) v;
}

gboolean
thrift_gdouble_equal (gconstpointer v1,
                      gconstpointer v2)
{
    return *((const gdouble*) v1) == *((const gdouble*) v2);
}

guint
thrift_gdouble_hash (gconstpointer v)
{
    return *(const gdouble*) v;
}

