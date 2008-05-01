/* 
 *
 */

#include "gen-c/thorough.h"
#include "gen-c/thorough_types.h"

#include <thrift.h>
#include <transport/thrift_framed.h>
#include <transport/thrift_socket.h>
#include <protocol/thrift_binary_protocol.h>

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

    // TODO: figure out why sending it as a property seg-faults
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
    //TEST_BASE (gint64, thrift_thorough_i64_i64, 5, ret == 5);
    //TEST_BASE (gdouble, thrift_thorough_double_double, 6, ret == 6);
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

    /*
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
                 simple_double, ret->value == simple_double->value);
    g_object_unref (simple_double);
     */

    /* TODO: thrift cpp boken with SimpleString...
    ThriftThoroughSimpleString * simple_string = g_object_new (THRIFT_THOROUGH_TYPE_SIMPLE_STRING, NULL);
    simple_string->value = "hello world";
    TEST_STRUCT (ThriftThoroughSimpleString *, 
                 thrift_thorough_simple_string__simple_string,
                 simple_string, ret->value == simple_string->value);
    g_object_unref (simple_string);
     */

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

    /* TODO: i64 and double support 
    ThriftThoroughSimpleAll * simple_all = g_object_new (THRIFT_THOROUGH_TYPE_SIMPLE_ALL, NULL);
    simple_all->value_bool = 1;
    simple_all->value_byte = 2;
    simple_all->value_i16 = 3;
    simple_all->value_i32 = 4;
    simple_all->value_i64 = 5;
    simple_all->value_double = 6.6;
    simple_all->value_string = "what";
    simple_all->value_typedef = "are you looking at";
    simple_all->value_exception->value->message = "rut-rho";
    TEST_STRUCT (ThriftThoroughSimpleAll *, 
                 thrift_thorough_simple_all__simple_all, simple_all, 
                 ((ret->value_bool == simple_all->value_bool) &&
                  (ret->value_byte == simple_all->value_byte) &&
                  (ret->value_i16 == simple_all->value_i16) &&
                  (ret->value_i32 == simple_all->value_i32) &&
                  (ret->value_i64 == simple_all->value_i64) &&
                  (ret->value_double == simple_all->value_double) &&
                  (ret->value_string == simple_all->value_string) &&
                  (ret->value_typedef == simple_all->value_typedef) &&
                  (ret->value_value_exception == simple_all->value_value_exception))
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
    numbering_all->value_typedef = "are you looking at";
    numbering_all->value_exception->value->message = "rut-rho";
    TEST_STRUCT (ThriftThoroughNumberingAll *, 
                 thrift_thorough_numbering_all__numbering_all, numbering_all, 
                 ((ret->value_bool == numbering_all->value_bool) &&
                  (ret->value_byte == numbering_all->value_byte) &&
                  (ret->value_i16 == numbering_all->value_i16) &&
                  (ret->value_i32 == numbering_all->value_i32) &&
                  (ret->value_i64 == numbering_all->value_i64) &&
                  (ret->value_double == numbering_all->value_double) &&
                  (ret->value_string == numbering_all->value_string) &&
                  (ret->value_typedef == numbering_all->value_typedef) &&
                  (ret->value_value_exception == numbering_all->value_value_exception))
                );
    g_object_unref (numbering_all);
     */

    ThriftThoroughHighMember * high_member = g_object_new (THRIFT_THOROUGH_TYPE_HIGH_MEMBER, NULL);
    high_member->value = 3;
    TEST_STRUCT (ThriftThoroughHighMember *, 
                 thrift_thorough_high_member__high_member,
                 high_member, ret->value == high_member->value);
    g_object_unref (high_member);

    /* leakin memory like a siev */
#define TEST_LIST(func, value, test) \
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
        g_ptr_array_add (val, 1);
        g_ptr_array_add (val, 42);
        g_ptr_array_add (val, 44);
        TEST_LIST (thrift_thorough_list_i32_list_i32, val, 
                   (
                    (g_ptr_array_index (val, 0) == g_ptr_array_index (ret, 0)) &&
                    (g_ptr_array_index (val, 1) == g_ptr_array_index (ret, 1)) &&
                    (g_ptr_array_index (val, 2) == g_ptr_array_index (ret, 2))
                   )
                  );
        g_ptr_array_free (val, 1);
    }

    {
        GPtrArray * val = g_ptr_array_new ();
        g_ptr_array_add (val, "hello");
        g_ptr_array_add (val, "world");
        g_ptr_array_add (val, "hrm");
        TEST_LIST (thrift_thorough_list_string_list_string, val, 
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
        GHashTable * val = g_hash_table_new (NULL, NULL);
        g_hash_table_insert (val, 1, NULL);
        g_hash_table_insert (val, 42, NULL);
        g_hash_table_insert (val, 44, NULL);
        TEST_HASH (thrift_thorough_set_i32_set_i32, val, 
                   (
                    g_hash_table_lookup (ret, 1) &&
                    g_hash_table_lookup (ret, 42) &&
                    g_hash_table_lookup (ret, 44)
                   )
                  );
        g_hash_table_destroy (val);
    }

    {
        GPtrArray * val = g_ptr_array_new ();
        g_ptr_array_add (val, "hello");
        g_ptr_array_add (val, "world");
        g_ptr_array_add (val, "hrm");
        TEST_LIST (thrift_thorough_list_string_list_string, val, 
                   (
                    (strcmp (g_ptr_array_index (val, 0), g_ptr_array_index (ret, 0)) == 0) &&
                    (strcmp (g_ptr_array_index (val, 1), g_ptr_array_index (ret, 1)) == 0) &&
                    (strcmp (g_ptr_array_index (val, 2), g_ptr_array_index (ret, 2)) == 0)
                   )
                  );
        g_ptr_array_free (val, 1);
    }

#if 0
    gboolean thrift_thorough_number_none (ThriftThoroughClient * client, const gint32 one, const gint32 two, GError ** error);
    gboolean thrift_thorough_number_complete (ThriftThoroughClient * client, const gint32 one, const gint32 two, GError ** error);
    gboolean thrift_thorough_number_skip (ThriftThoroughClient * client, const gint32 one, const gint32 two, GError ** error);
    gboolean thrift_thorough_number_partial (ThriftThoroughClient * client, const gint32 one, const gint32 two, GError ** error);
    gboolean thrift_thorough_number_all (ThriftThoroughClient * client, const gint32 one, const gint32 two, GError ** error);

    gboolean thrift_thorough_set_i32_set_i32 (ThriftThoroughClient * client, GHashTable ** _return, const GHashTable * param, GError ** error);
    gboolean thrift_thorough_set_simple_set_simple (ThriftThoroughClient * client, GHashTable ** _return, const GHashTable * param, GError ** error);
    gboolean thrift_thorough_map_i32_map_i32 (ThriftThoroughClient * client, GHashTable ** _return, const GHashTable * param, GError ** error);
    gboolean thrift_thorough_map_simple_map_simple (ThriftThoroughClient * client, GHashTable ** _return, const GHashTable * param, GError ** error);
    gboolean thrift_thorough_throws_simple (ThriftThoroughClient * client, GError ** error);
    gboolean thrift_thorough_throws_complex (ThriftThoroughClient * client, GError ** error);
    gboolean thrift_thorough_throws_multiple_simple (ThriftThoroughClient * client, GError ** error);
    gboolean thrift_thorough_throws_multiple_complex (ThriftThoroughClient * client, GError ** error);
#endif

    return 0;
}
