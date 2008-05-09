/* 
 *
 */

#include "gen-c/thorough.h"
#include "gen-c/thorough_types.h"

#include <thrift.h>
#include <thrift_hash.h>
#include <transport/thrift_framed.h>
#include <transport/thrift_socket.h>
#include <protocol/thrift_binary_protocol.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HOST "localhost"
#define PORT 9090

int main (int argc, char * argv[])
{
    THRIFT_UNUSED_VAR (argc);
    THRIFT_UNUSED_VAR (argv);

    g_type_init ();

    ThriftSocket * socket = g_object_new (THRIFT_TYPE_SOCKET,
                                          "hostname", HOST,
                                          "port", PORT, 
                                          NULL);

    ThriftFramed * framed = g_object_new (THRIFT_TYPE_FRAMED,
                                          "transport", socket,
                                          NULL);

    ThriftBinaryProtocol * protocol = g_object_new (THRIFT_TYPE_BINARY_PROTOCOL, 
                                                    "transport", framed, 
                                                    NULL);

    ThriftThoroughClient * client = g_object_new (THRIFT_THOROUGH_TYPE_CLIENT,
                                                  "protocol", protocol, 
                                                  NULL);

    GError * err = NULL;
    if (!thrift_transport_open (THRIFT_TRANSPORT (socket), &err))
    {
        fprintf (stderr, "failed to connect to host");
        exit (-1);
    }

    GError * error = NULL;

    if (!thrift_thorough_void_void (client, &error))
    {
        fprintf (stderr, "thrift_thorough_void_void failed: %s", 
                 error ? error->message : "(unknown)");
        exit (-1);
    }

#define TEST_BASE(ctype, func, value, test) \
    { \
        ctype ret; \
        if (!func (client, &ret, value, &error)) \
        { \
            fprintf (stderr, "%s failed: %s\n", #func, \
                     error ? error->message : "(unknown)"); \
            exit (-1); \
        } \
        else if (!(test)) \
        { \
            fprintf (stderr, "%s failed\n", #test); \
            exit (-1); \
        } \
    }

    TEST_BASE (gboolean, thrift_thorough_bool_bool, 1, ret == 1);
    TEST_BASE (gint8, thrift_thorough_byte_byte, 2, ret == 2);
    TEST_BASE (gint16, thrift_thorough_i16_i16, 3, ret == 3);
    TEST_BASE (gint32, thrift_thorough_i32_i32, 4, ret == 4);
    TEST_BASE (gint64, thrift_thorough_i64_i64, 5, ret == 5);
    TEST_BASE (gdouble, thrift_thorough_double_double, 6.6, fabs (ret - 6.6) < 0.001);
    TEST_BASE (gchar *, thrift_thorough_string_string, "hey", strcmp (ret, "hey") == 0);

    TEST_BASE (ThriftThoroughComplete, thrift_thorough_complete__complete,
               THRIFT_THOROUGH_COMPLETE_HORSE,
               ret == THRIFT_THOROUGH_COMPLETE_HORSE);

#define TEST_STRUCT(ctype, func, value, test) \
    { \
        ctype ret = NULL; \
        if (!func (client, &ret, value, &error)) \
        { \
            fprintf (stderr, "%s failed: %s\n", #func, \
                     error ? error->message : "(unknown)"); \
            exit (-1); \
        } \
        else if (!(test)) \
        { \
            fprintf (stderr, "%s failed\n", #test); \
            exit (-1); \
        } \
        g_object_unref (ret); \
    }

    ThriftThoroughSimpleBool * simple_bool = g_object_new (THRIFT_THOROUGH_TYPE_SIMPLE_BOOL, NULL);
    simple_bool->value = 0;
    TEST_STRUCT (ThriftThoroughSimpleBool *, 
                 thrift_thorough_simple_bool__simple_bool,
                 simple_bool, ret->value == simple_bool->value);
    g_object_unref (simple_bool);

    ThriftThoroughSimpleByte * simple_byte = g_object_new (THRIFT_THOROUGH_TYPE_SIMPLE_BYTE, NULL);
    simple_byte->value = 1;
    TEST_STRUCT (ThriftThoroughSimpleByte *, 
                 thrift_thorough_simple_byte__simple_byte,
                 simple_byte, ret->value == simple_byte->value);
    g_object_unref (simple_byte);

    ThriftThoroughSimpleI16 * simple_i16 = g_object_new (THRIFT_THOROUGH_TYPE_SIMPLE_I16, NULL);
    simple_i16->value = 2;
    TEST_STRUCT (ThriftThoroughSimpleI16 *, 
                 thrift_thorough_simple_i16__simple_i16,
                 simple_i16, ret->value == simple_i16->value);
    g_object_unref (simple_i16);

    ThriftThoroughSimpleI32 * simple_i32 = g_object_new (THRIFT_THOROUGH_TYPE_SIMPLE_I32, NULL);
    simple_i32->value = 3;
    TEST_STRUCT (ThriftThoroughSimpleI32 *, 
                 thrift_thorough_simple_i32__simple_i32,
                 simple_i32, ret->value == simple_i32->value);
    g_object_unref (simple_i32);

    ThriftThoroughSimpleI64 * simple_i64 = g_object_new (THRIFT_THOROUGH_TYPE_SIMPLE_I64, NULL);
    simple_i64->value = 4;
    TEST_STRUCT (ThriftThoroughSimpleI64 *, 
                 thrift_thorough_simple_i64__simple_i64,
                 simple_i64, ret->value == simple_i64->value);
    g_object_unref (simple_i64);

    ThriftThoroughSimpleDouble * simple_double = g_object_new (THRIFT_THOROUGH_TYPE_SIMPLE_DOUBLE, NULL);
    simple_double->value = 5.5;
    TEST_STRUCT (ThriftThoroughSimpleDouble *, 
                 thrift_thorough_simple_double__simple_double,
                 simple_double, fabs (ret->value - simple_double->value) < 0.001);
    g_object_unref (simple_double);

    ThriftThoroughSimpleString * simple_string = g_object_new (THRIFT_THOROUGH_TYPE_SIMPLE_STRING, NULL);
    simple_string->value = "hello world";
    TEST_STRUCT (ThriftThoroughSimpleString *, 
                 thrift_thorough_simple_string__simple_string,
                 simple_string, strcmp (ret->value, simple_string->value) == 0);
    g_object_unref (simple_string);

    ThriftThoroughSimpleTypedef * simple_typedef = g_object_new (THRIFT_THOROUGH_TYPE_SIMPLE_TYPEDEF, NULL);
    simple_typedef->value = "hello world";
    TEST_STRUCT (ThriftThoroughSimpleTypedef *, 
                 thrift_thorough_simple_typedef__simple_typedef,
                 simple_typedef, strcmp (ret->value, simple_typedef->value) == 0);
    g_object_unref (simple_typedef);

    ThriftThoroughSimpleEnum * simple_enum = g_object_new (THRIFT_THOROUGH_TYPE_SIMPLE_ENUM, NULL);
    simple_enum->value = THRIFT_THOROUGH_COMPLETE_DOG;
    TEST_STRUCT (ThriftThoroughSimpleEnum *, 
                 thrift_thorough_simple_enum__simple_enum,
                 simple_enum, ret->value == simple_enum->value);
    g_object_unref (simple_enum);

    {
        ThriftThoroughSimpleException * ret = NULL;
        if (!thrift_thorough_void__simple_exception (client, &ret, &error))
        {
            fprintf (stderr, "%s failed: %s\n", 
                     "thrift_thorough_void__simple_exception",
                     error ? error->message : "(unknown)");
            exit (-1);
        }
        else if (ret == NULL || 
                 strcmp ("simple exception", ret->value->message) != 0)
        {
            fprintf (stderr, "%s failed\n",
                     "thrift_thorough_void__simple_exception");
            exit (-1);
        }
        if (ret != NULL) g_object_unref (ret);
    }

    ThriftThoroughSimpleAll * simple_all = g_object_new (THRIFT_THOROUGH_TYPE_SIMPLE_ALL, NULL);
    simple_all->value_bool = 1;
    simple_all->value_byte = 2;
    simple_all->value_i16 = 3;
    simple_all->value_i32 = 4;
    simple_all->value_i64 = 5;
    simple_all->value_double = 6.6;
    simple_all->value_string = "what";
    simple_all->value_typedef = 7.7;
    TEST_STRUCT (ThriftThoroughSimpleAll *, 
                 thrift_thorough_simple_all__simple_all, simple_all, 
                 (
                  (ret->value_bool == simple_all->value_bool) &&
                  (ret->value_byte == simple_all->value_byte) &&
                  (ret->value_i16 == simple_all->value_i16) &&
                  (ret->value_i32 == simple_all->value_i32) &&
                  (ret->value_i64 == simple_all->value_i64) &&
                  (fabs (ret->value_double - simple_all->value_double) < 0.001) &&
                  (strcmp (ret->value_string, simple_all->value_string) == 0) &&
                  (ret->value_typedef == simple_all->value_typedef)
                 )
                );
    g_object_unref (simple_all);

    ThriftThoroughNumberingAll * numbering_all = g_object_new (THRIFT_THOROUGH_TYPE_NUMBERING_ALL, NULL);
    numbering_all->value_bool = 1;
    numbering_all->value_byte = 2;
    numbering_all->value_i16 = 3;
    numbering_all->value_i32 = 4;
    numbering_all->value_i64 = 5;
    numbering_all->value_double = 6.6;
    numbering_all->value_string = "what";
    numbering_all->value_typedef = 7.7;
    TEST_STRUCT (ThriftThoroughNumberingAll *, 
                 thrift_thorough_numbering_all__numbering_all, numbering_all, 
                 (
                  (ret->value_bool == numbering_all->value_bool) &&
                  (ret->value_byte == numbering_all->value_byte) &&
                  (ret->value_i16 == numbering_all->value_i16) &&
                  (ret->value_i32 == numbering_all->value_i32) &&
                  (ret->value_i64 == numbering_all->value_i64) &&
                  (fabs (ret->value_double - numbering_all->value_double) < 0.001) &&
                  (strcmp (ret->value_string, numbering_all->value_string) == 0) &&
                  (ret->value_typedef == numbering_all->value_typedef)
                 )
                );
    g_object_unref (numbering_all);

    ThriftThoroughHighMember * high_member = g_object_new (THRIFT_THOROUGH_TYPE_HIGH_MEMBER, NULL);
    high_member->value = 3;
    TEST_STRUCT (ThriftThoroughHighMember *, 
                 thrift_thorough_high_member__high_member,
                 high_member, ret->value == high_member->value);
    g_object_unref (high_member);

    /* leakin memory like a siev */
#define TEST_LIST(func, value, test) \
    { \
        GArray * ret = NULL; \
        if (!func (client, &ret, value, &error)) \
        { \
            fprintf (stderr, "%s failed: %s\n", #func, \
                     error ? error->message : "(unknown)"); \
            exit (-1); \
        } \
        else if (!(test)) \
        { \
            fprintf (stderr, "%s failed\n", #test); \
            exit (-1); \
        } \
        g_array_free (ret, 1); \
    }

    {
        GArray * val = g_array_new (0, 1, sizeof (gint32));
        gint32 v = 1;
        g_array_append_val (val, v);
        v = 42;
        g_array_append_val (val, v);
        v = 44;
        g_array_append_val (val, v);
        TEST_LIST (thrift_thorough_list_i32_list_i32, val, 
                   (
                    (g_array_index (val, gint32, 0) == g_array_index (ret, gint32, 0)) &&
                    (g_array_index (val, gint32, 1) == g_array_index (ret, gint32, 1)) &&
                    (g_array_index (val, gint32, 2) == g_array_index (ret, gint32, 2))
                   )
                  );
        g_array_free (val, 1);
    }

#define TEST_PLIST(func, value, test) \
    { \
        GPtrArray * ret = NULL; \
        if (!func (client, &ret, value, &error)) \
        { \
            fprintf (stderr, "%s failed: %s\n", #func, \
                     error ? error->message : "(unknown)"); \
            exit (-1); \
        } \
        else if (!(test)) \
        { \
            fprintf (stderr, "%s failed\n", #test); \
            exit (-1); \
        } \
        g_ptr_array_free (ret, 1); \
    }

    {
        GPtrArray * val = g_ptr_array_new ();
        g_ptr_array_add (val, "hello");
        g_ptr_array_add (val, "world");
        g_ptr_array_add (val, "hrm");
        TEST_PLIST (thrift_thorough_list_string_list_string, val, 
                    (
                     (strcmp (g_ptr_array_index (val, 0), g_ptr_array_index (ret, 0)) == 0) &&
                     (strcmp (g_ptr_array_index (val, 1), g_ptr_array_index (ret, 1)) == 0) &&
                     (strcmp (g_ptr_array_index (val, 2), g_ptr_array_index (ret, 2)) == 0)
                    )
                   );
        g_ptr_array_free (val, 1);
    }

    /* leakin memory like a siev */
#define TEST_HASH(func, value, test) \
    { \
        GHashTable * ret = NULL; \
        if (!func (client, &ret, value, &error)) \
        { \
            fprintf (stderr, "%s failed: %s\n", #func, \
                     error ? error->message : "(unknown)"); \
            exit (-1); \
        } \
        else if (!(test)) \
        { \
            fprintf (stderr, "%s failed\n", #test); \
            exit (-1); \
        } \
        g_hash_table_destroy (ret); \
    }

    {
        GHashTable * val = g_hash_table_new (thrift_gint32_hash, 
                                             thrift_gint32_equal);
        gint32 * e1 = g_new (gint32, 1);
        *e1 = 1;
        g_hash_table_insert (val, (gpointer)e1, (gpointer)1);
        gint32 * e2 = g_new (gint32, 1);
        *e2 = 42;
        g_hash_table_insert (val, (gpointer)e2, (gpointer)1);
        gint32 * e3 = g_new (gint32, 1);
        *e3 = 44;
        g_hash_table_insert (val, (gpointer)e3, (gpointer)1);
        TEST_HASH (thrift_thorough_set_i32_set_i32, val, 
                   (
                    g_hash_table_lookup (ret, (gpointer)e1) != NULL &&
                    g_hash_table_lookup (ret, (gpointer)e2) != NULL &&
                    g_hash_table_lookup (ret, (gpointer)e3) != NULL
                   )
                  );
        g_hash_table_destroy (val);
    }

    {
        GHashTable * val = g_hash_table_new (g_str_hash, g_str_equal);
        g_hash_table_insert (val, "hello", (gpointer)1);
        g_hash_table_insert (val, "world", (gpointer)1);
        g_hash_table_insert (val, "huh", (gpointer)1);
        TEST_HASH (thrift_thorough_set_string_set_string, val, 
                   (
                    g_hash_table_lookup (ret, "hello") != NULL &&
                    g_hash_table_lookup (ret, "world") != NULL &&
                    g_hash_table_lookup (ret, "huh") != NULL
                   )
                  );
        g_hash_table_destroy (val);
    }

    {
        GHashTable * val = g_hash_table_new (thrift_gint32_hash, 
                                             thrift_gint32_equal);
        gint32 * k1 = g_new (gint32, 1);
        gint32 * v1 = g_new (gint32, 1);
        *k1 = 1;
        *v1 = 2;
        g_hash_table_insert (val, (gpointer)k1, (gpointer)v1);
        gint32 * k2 = g_new (gint32, 1);
        gint32 * v2 = g_new (gint32, 1);
        *k2 = 42;
        *v2 = 43;
        g_hash_table_insert (val, (gpointer)k2, (gpointer)v2);
        gint32 * k3 = g_new (gint32, 1);
        gint32 * v3 = g_new (gint32, 1);
        *k3 = 44;
        *v3 = 45;
        g_hash_table_insert (val, (gpointer)k3, (gpointer)v3);
        TEST_HASH (thrift_thorough_map_i32_map_i32, val, 
                   (
                    (*((gint32*)g_hash_table_lookup (ret, (gpointer)k1)) == *v1) &&
                    (*((gint32*)g_hash_table_lookup (ret, (gpointer)k2)) == *v2) &&
                    (*((gint32*)g_hash_table_lookup (ret, (gpointer)k3)) == *v3)
                   )
                  );
        g_hash_table_destroy (val);
    }

    {
        GHashTable * val = g_hash_table_new (g_str_hash, g_str_equal);
        g_hash_table_insert (val, "hello", "world");
        g_hash_table_insert (val, "world", "huh");
        g_hash_table_insert (val, "huh", "bye");
        TEST_HASH (thrift_thorough_map_string_map_string, val, 
                   (
                    (strcmp (g_hash_table_lookup (ret, "hello"), "world") == 0) &&
                    (strcmp (g_hash_table_lookup (ret, "world"), "huh") == 0) &&
                    (strcmp (g_hash_table_lookup (ret, "huh"), "bye") == 0)
                   )
                  );
        g_hash_table_destroy (val);
    }

    {
        GHashTable * val = g_hash_table_new (g_str_hash, g_str_equal);
        ThriftThoroughSimpleI32 * sd1 = g_object_new (THRIFT_THOROUGH_TYPE_SIMPLE_I32, 
                                                      NULL);
        sd1->value = 1;
        g_hash_table_insert (val, "hello", sd1);
        ThriftThoroughSimpleI32 * sd2 = g_object_new (THRIFT_THOROUGH_TYPE_SIMPLE_I32, 
                                                      NULL);
        sd2->value = 2;
        g_hash_table_insert (val, "world", sd2);
        ThriftThoroughSimpleI32 * sd3 = g_object_new (THRIFT_THOROUGH_TYPE_SIMPLE_I32, 
                                                      NULL);
        sd3->value = 3;
        g_hash_table_insert (val, "huh", sd3);
        TEST_HASH (thrift_thorough_map_string__simple_i32_map_string__simple_i32,
                   val, 
                   (
                    (((ThriftThoroughSimpleI32*)g_hash_table_lookup (ret, "hello"))->value == sd1->value) &&
                    (((ThriftThoroughSimpleI32*)g_hash_table_lookup (ret, "world"))->value == sd2->value) &&
                    (((ThriftThoroughSimpleI32*)g_hash_table_lookup (ret, "huh"))->value == sd3->value)
                   )
                  );
        g_hash_table_destroy (val);
    }

#define TEST_EX(func, value) \
    { \
        GError * ex = NULL; \
        if (!func (client, &ex)) \
        { \
            if (!ex || !ex->message) \
            { \
                fprintf (stderr, "%s failed: no error\n", #func); \
            } \
            else if (strcmp (value, ex->message) != 0) \
            { \
                fprintf (stderr, "%s failed: unexpected message, expected %s got %s\n", \
                         #func, value, ex->message); \
                g_error_free (ex); \
            } \
        } \
        else \
        { \
            fprintf (stderr, "%s failed: unexpected success\n", #func); \
        } \
    }

    TEST_EX (thrift_thorough_throws_simple, "simple exception");

    TEST_EX (thrift_thorough_throws_complex, "complex exception");

    TEST_EX (thrift_thorough_throws_multiple_simple, "simple exception");

    TEST_EX (thrift_thorough_throws_multiple_complex, "complex exception");

#if 0
    gboolean thrift_thorough_number_none (ThriftThoroughClient * client, const gint32 one, const gint32 two, GError ** error);
    gboolean thrift_thorough_number_complete (ThriftThoroughClient * client, const gint32 one, const gint32 two, GError ** error);
    gboolean thrift_thorough_number_skip (ThriftThoroughClient * client, const gint32 one, const gint32 two, GError ** error);
    gboolean thrift_thorough_number_partial (ThriftThoroughClient * client, const gint32 one, const gint32 two, GError ** error);
    gboolean thrift_thorough_number_all (ThriftThoroughClient * client, const gint32 one, const gint32 two, GError ** error);
#endif

    return 0;
}
