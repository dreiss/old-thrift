// Half of this file comes from contributions from Nitay Joffe (nitay@powerset.com)
// Much of the rest (almost) directly ported (or pulled) from thrift-py's fastbinary.c
// Everything else via Kevin Clark (kevin@powerset.com)
#include <stdint.h>
#include <stdbool.h>

#include <ruby.h>
#include <st.h>

// #define __DEBUG__

#define dbg() fprintf(stderr, "%s:%d\n", __FUNCTION__, __LINE__)


// Stolen out of TProtocol.h.
// It would be a huge pain to have both get this from one place.

enum TType {
  T_STOP       = 0,
  T_BOOL       = 2,
  T_BYTE       = 3,
  T_I16        = 6,
  T_I32        = 8,
  T_I64        = 10,
  T_DBL        = 4,
  T_STR        = 11,
  T_STRCT      = 12,
  T_MAP        = 13,
  T_SET        = 14,
  T_LIST       = 15
  // T_VOID       = 1,
  // T_I08        = 3,
  // T_U64        = 9,
  // T_UTF7       = 11,
  // T_UTF8       = 16,
  // T_UTF16      = 17
};

// Same comment as the enum.  Sorry.
#ifdef HAVE_ENDIAN_H
#include <endian.h>
#endif

#ifndef __BYTE_ORDER
# if defined(BYTE_ORDER) && defined(LITTLE_ENDIAN) && defined(BIG_ENDIAN)
#  define __BYTE_ORDER BYTE_ORDER
#  define __LITTLE_ENDIAN LITTLE_ENDIAN
#  define __BIG_ENDIAN BIG_ENDIAN
# else
#  error "Cannot determine endianness"
# endif
#endif

#if __BYTE_ORDER == __BIG_ENDIAN
# define ntohll(n) (n)
# define htonll(n) (n)
#elif __BYTE_ORDER == __LITTLE_ENDIAN
# if defined(__GNUC__) && defined(__GLIBC__)
#  include <byteswap.h>
#  define ntohll(n) bswap_64(n)
#  define htonll(n) bswap_64(n)
# else /* GNUC & GLIBC */
#  define ntohll(n) ( (((unsigned long long)ntohl(n)) << 32) + ntohl(n >> 32) )
#  define htonll(n) ( (((unsigned long long)htonl(n)) << 32) + htonl(n >> 32) )
# endif /* GNUC & GLIBC */
#else /* __BYTE_ORDER */
# error "Can't define htonll or ntohll!"
#endif


static VALUE class_tfbp;
static ID type_sym;
static ID class_sym;
static ID key_sym;
static ID value_sym;
static ID element_sym;
static ID name_sym;
static ID fields_id;
static ID consume_bang_id;
static ID string_buffer_id;
static ID refill_buffer_id;

static const uint32_t VERSION_MASK = 0xffff0000;
static const uint32_t VERSION_1 = 0x80010000;

// -----------------------------------------------------------------------------
// Structs so I don't have to keep calling rb_hash_aref
// -----------------------------------------------------------------------------

// { :type => field[:type],
//   :class => field[:class],
//   :key => field[:key],
//   :value => field[:value],
//   :element => field[:element] }

struct _thrift_map;
struct _field_spec;

typedef union {
  VALUE class;
  struct _thrift_map* map;
  struct _field_spec* element;
} container_data;

typedef struct _field_spec {
  int type;
  char* name;
  container_data data;
} field_spec;

typedef struct _thrift_map {
  field_spec* key;
  field_spec* value; 
} thrift_map;


static void free_field_spec(field_spec* spec) {
  switch(spec->type) {
    case T_STRCT:
      free(spec);
      break;
    
    case T_LIST:
    case T_SET:
      free_field_spec(spec->data.element);
      free(spec);
      break;
    
    case T_MAP:
      free_field_spec(spec->data.map->key);
      free_field_spec(spec->data.map->value);
      free(spec->data.map);
      free(spec);
      break;
  }
}

static field_spec* parse_field_spec(VALUE field_data) {
  int type = NUM2INT(rb_hash_aref(field_data, type_sym));
  VALUE name = rb_hash_aref(field_data, name_sym);
  field_spec* spec = (field_spec *) malloc(sizeof(field_spec));

#ifdef __DEBUG__ // No need for this in prod since I set all the fields
  bzero(spec, sizeof(field_spec));
#endif

  spec->type = type;
  
  if (Qnil != name) {
    spec->name = StringValuePtr(name);
  } else {
    spec->name = NULL;
  }
  
  switch(type) {
    case T_STRCT: {
      spec->data.class = rb_hash_aref(field_data, class_sym);
      break;
    }
    
    case T_MAP: {
      VALUE key_fields = rb_hash_aref(field_data, key_sym);
      VALUE value_fields = rb_hash_aref(field_data, value_sym);
      thrift_map* map = (thrift_map *) malloc(sizeof(thrift_map));
      
      map->key = parse_field_spec(key_fields);
      map->value = parse_field_spec(value_fields);
      spec->data.map = map;
      
      break;
    }
    
    case T_LIST: 
    case T_SET:
    {
      VALUE list_fields = rb_hash_aref(field_data, element_sym);
      spec->data.element = parse_field_spec(list_fields);
      break;
    }
  }
  
  return spec;
}


// -----------------------------------------------------------------------------
// Write output stuff
// -----------------------------------------------------------------------------

static void write_byte(VALUE buf, int8_t val) {
  rb_str_buf_cat(buf, (char*)&val, sizeof(int8_t));
}

static void write_i16(VALUE buf, int16_t val) {
  int16_t net = (int16_t)htons(val);
  rb_str_buf_cat(buf, (char*)&net, sizeof(int16_t));
}

static void write_i32(VALUE buf, int32_t val) {
  int32_t net = (int32_t)htonl(val);
  rb_str_buf_cat(buf, (char*)&net, sizeof(int32_t));
}

static void write_i64(VALUE buf, int64_t val) {
  int64_t net = (int64_t)htonll(val);
  rb_str_buf_cat(buf, (char*)&net, sizeof(int64_t));
}

static void write_double(VALUE buf, double dub) {
  // Unfortunately, bitwise_cast doesn't work in C.  Bad C!
  union {
    double f;
    int64_t t;
  } transfer;
  transfer.f = dub;
  write_i64(buf, transfer.t);
}

static void write_string(VALUE buf, char* str) {
  int32_t len = strlen(str);
  write_i32(buf, len);
  rb_str_buf_cat2(buf, str);
}

#define write_struct_begin(buf)
#define write_struct_end(buf)

static void write_field_begin(VALUE buf, char* name, int type, int fid) {
#ifdef __DEBUG__
  fprintf(stderr, "Writing field beginning: %s %d %d\n", name, type, fid);
#endif

  write_byte(buf, (int8_t)type);
  write_i16(buf, (int16_t)fid);
}

#define write_field_end(buf)

static void write_field_stop(VALUE buf) {
  write_byte(buf, T_STOP);
}

static void write_map_begin(VALUE buf, int8_t ktype, int8_t vtype, int32_t sz) {
  write_byte(buf, ktype);
  write_byte(buf, vtype);
  write_i32(buf, sz);
}

#define write_map_end(buf);

static void write_list_begin(VALUE buf, int type, int sz) {
  write_byte(buf, type);
  write_i32(buf, sz);
}

#define write_list_end(buf)

static void write_set_begin(VALUE buf, int type, int sz) {
  write_byte(buf, type);
  write_i32(buf, sz);
}

#define write_set_end(buf)

static void binary_encoding(VALUE buf, VALUE obj, int type);

static int write_map_data(VALUE key, VALUE val, VALUE ary) {
  VALUE buf = rb_ary_entry(ary, 0);
  int key_type = FIX2INT(rb_ary_entry(ary, 1));
  int val_type = FIX2INT(rb_ary_entry(ary, 2));
  
  binary_encoding(buf, key, key_type);
  binary_encoding(buf, val, val_type);
  
  return 0;
}

static int write_set_data(VALUE val, VALUE truth, VALUE ary) {
  VALUE buf = rb_ary_entry(ary, 0);
  int type = FIX2INT(rb_ary_entry(ary, 1));
  
  binary_encoding(buf, val, type);
  
  return 0;
}

static void write_container(VALUE buf, VALUE value, field_spec* spec) {
  int sz;
  
  switch(spec->type) {
    case T_MAP: {
      sz = RHASH(value)->tbl->num_entries;
      
      // TODO(kevinclark): Yuck. Yuck yuck yuck. Figure out how to not convert types over and over. 
      VALUE args = rb_ary_new3(3, buf, INT2FIX(spec->data.map->key->type), INT2FIX(spec->data.map->value->type));
      
      write_map_begin(buf, spec->data.map->key->type, spec->data.map->value->type, sz);
      rb_hash_foreach(value, write_map_data, args);
      write_map_end(buf);
      break;
    }
    
    case T_LIST: {
      sz = RARRAY(value)->len;
      int i;
      
      write_list_begin(buf, spec->data.element->type, sz);
      for (i = 0; i < sz; ++i) {
        binary_encoding(buf, rb_ary_entry(value, i), spec->data.element->type);
      }
      write_list_end(buf);
      break;
    }
    
    case T_SET: {
      sz = RHASH(value)->tbl->num_entries;
      
      VALUE args = rb_ary_new3(2, buf, INT2FIX(spec->data.element->type));
      write_set_begin(buf, spec->data.element->type, sz);
      rb_hash_foreach(value, write_set_data, args);
      write_set_end(buf);
      break;
    }
  }
}

#define IS_CONTAINER(x) (x == T_MAP || x == T_SET || x == T_LIST)

static int encode_field(VALUE fid, VALUE data, VALUE ary) {
  field_spec *spec = parse_field_spec(data);
  
  VALUE buf = rb_ary_entry(ary, 0);
  VALUE obj = rb_ary_entry(ary, 1);
  char name_buf[128];
  
  name_buf[0] = '@';
  strlcpy(&name_buf[1], spec->name, sizeof(name_buf) - 1);
  
  // TODO(kevinclark): Replace with the strlcpy method used in read
  VALUE value = rb_ivar_get(obj, rb_intern(name_buf));
  
  if (Qnil == value)
    return 0;
     
  write_field_begin(buf, spec->name, spec->type, NUM2INT(fid));
  
  if (IS_CONTAINER(spec->type)) {
    write_container(buf, value, spec);
  } else {
    binary_encoding(buf, value, spec->type);
  }
  write_field_end(buf);
  
  free_field_spec(spec);
  
  return 0;
}

// -----------------------------------------------------------------------------
// TFastBinaryProtocol's main encoding loop
// -----------------------------------------------------------------------------

static void binary_encoding(VALUE buf, VALUE obj, int type) {
#ifdef __DEBUG__
  rb_p(rb_str_new2("Encoding binary (buf, obj, type)"));
  rb_p(rb_inspect(buf));
  rb_p(rb_inspect(obj));
  rb_p(rb_inspect(INT2FIX(type)));
#endif

  switch(type) {
    case T_BOOL:
      if RTEST(obj) {
        write_byte(buf, 1);
      }
      else {
        write_byte(buf, 0);
      }
      
      break;
    
    case T_BYTE:
      write_byte(buf, NUM2INT(obj));
      break;
    
    case T_I16:
      write_i16(buf, NUM2INT(obj));
      break;
    
    case T_I32:
      write_i32(buf, NUM2INT(obj));
      break;
    
    case T_I64: {
      int64_t val;
      switch (TYPE(obj)) {
        case T_FIXNUM:
          val = NUM2INT(obj);
          break;
        case T_BIGNUM:
          val = rb_num2ll(obj);
          break;
        default:
          rb_raise(rb_eArgError, "Argument is not a Fixnum or Bignum");
      }

      write_i64(buf, val);
      break;
    }
    
    case T_DBL:
      write_double(buf, NUM2DBL(obj));
      break;

    case T_STR:
      write_string(buf, StringValuePtr(obj));
      break;
          
    case T_STRCT: {      
      VALUE args = rb_ary_new3(2, buf, obj);
      VALUE fields = rb_const_get(CLASS_OF(obj), fields_id);
      
      write_struct_begin(buf);
      
      rb_hash_foreach(fields, encode_field, args);
      
      write_field_stop(buf);
      write_struct_end(buf);
      break;
    }
  }
}

// obj is always going to be a TSTRCT
VALUE tfbp_encode_binary(VALUE self, VALUE obj) {
  VALUE buf = rb_str_buf_new(1024);
  binary_encoding(buf, obj, T_STRCT);
  return buf;
}


// -----------------------------------------------------------------------------
// Read stuff
// -----------------------------------------------------------------------------

typedef struct {
  char* name;
  int8_t type;
  int16_t id;
} field_header;

typedef struct {
  int key_type;
  int val_type;
  int num_entries;
} map_header;

typedef struct {
  int type;
  int num_elements;
} list_header;

typedef list_header set_header;

typedef struct {
  char* data;
  int pos;
  int len;
  VALUE trans;
} decode_buffer;

typedef struct {
  char* ptr;
  int len;
} thrift_string;

#define read_struct_begin(buf)
#define read_struct_end(buf)

static bool read_bytes(decode_buffer* buf, void* dst, size_t size) {
  int avail = (buf->len - buf->pos);
  
  if (size <= avail) {
    memcpy(dst, buf->data + buf->pos, size);
    buf->pos += size;
  } else {
    rb_funcall(buf->trans, consume_bang_id, 1, INT2FIX(avail));
    VALUE refill = rb_funcall(buf->trans, refill_buffer_id, 1, INT2FIX(size));
    // Copy what we can
    memcpy(dst, buf->data, avail);
    // Refill the buffer
    buf->data = StringValuePtr(refill);
    buf->len = RSTRING(refill)->len;
    buf->pos = size - avail;
    memcpy(dst + avail, buf->data, buf->pos);
  }
  
  return true;
}

static int8_t read_byte(decode_buffer* buf) {
  int8_t data;
  read_bytes(buf, &data, sizeof(int8_t));
  return data;
}

static int16_t read_int16(decode_buffer* buf) {
  int16_t data;
  read_bytes(buf, &data, sizeof(int16_t));
  return ntohs(data);
}

static int32_t read_int32(decode_buffer* buf) {
  int32_t data;
  read_bytes(buf, &data, sizeof(int32_t));
  return ntohl(data);
}

static int64_t read_int64(decode_buffer* buf) {
  int64_t data;
  read_bytes(buf, &data, sizeof(int64_t));
  return ntohll(data);
}

static double read_double(decode_buffer* buf) {
  union {
    double f;
    int64_t t;
  } transfer;
  
  transfer.t = read_int64(buf);
  return transfer.f;
}

static thrift_string read_string(decode_buffer* buf) {
  thrift_string data;
  data.len = read_int32(buf);
  data.ptr = buf->data + buf->pos;
  buf->pos += data.len;
  
  return data;
}

static void read_field_begin(decode_buffer* buf, field_header* header) {
#ifdef __DEBUG__ // No need for this in prod since I set all the fields
  bzero(header, sizeof(field_header));
#endif

  header->name = NULL;
  header->type = read_byte(buf);
  if (header->type == T_STOP) {
    header->id = 0;
  } else {
    header->id = read_int16(buf);
  }
}

#define read_field_end(buf)

static void read_map_begin(decode_buffer* buf, map_header* header) {
#ifdef __DEBUG__ // No need for this in prod since I set all the fields
  bzero(header, sizeof(map_header));
#endif
  
  header->key_type = read_byte(buf);
  header->val_type = read_byte(buf);
  header->num_entries = read_int32(buf);
}

#define read_map_end(buf)

static void read_list_begin(decode_buffer* buf, list_header* header) {
#ifdef __DEBUG__ // No need for this in prod since I set all the fields
  bzero(header, sizeof(list_header));
#endif
  
  header->type = read_byte(buf);
  header->num_elements = read_int32(buf);
}

#define read_list_end(buf)

#define read_set_begin read_list_begin
#define read_set_end read_list_end

static VALUE read_type(int type, decode_buffer* buf) {
  switch(type) {
    case T_BOOL: {
      int8_t byte = read_byte(buf);
      if (0 == byte) {
        return Qfalse;
      } else {
        return Qtrue;
      }
    }
    
    case T_BYTE:
      return INT2FIX(read_byte(buf));
    
    case T_I16:
      return INT2FIX(read_int16(buf));

    case T_I32:
      return INT2NUM(read_int32(buf));

    case T_I64:
      return rb_ll2inum(read_int64(buf));
    
    case T_DBL:
      return rb_float_new(read_double(buf));

    case T_STR: {
      thrift_string str = read_string(buf);
      return rb_str_new(str.ptr, str.len);
    }
  }
  
  return Qnil;
}

static void skip_type(int type, decode_buffer* buf) {
  read_type(type, buf);
}


VALUE read_struct(VALUE obj, decode_buffer* buf);

static VALUE read_field(decode_buffer* buf, field_spec* spec) {
  switch (spec->type) {
    case T_STRCT: {
      VALUE obj = rb_class_new_instance(0, NULL, spec->data.class);
      return read_struct(obj, buf);
    }
    
    case T_MAP: {
      map_header hdr;
      VALUE hsh;
      int i;
      
      read_map_begin(buf, &hdr); 
      hsh = rb_hash_new();
      
      for (i = 0; i < hdr.num_entries; ++i) {
        VALUE key = read_field(buf, spec->data.map->key);
        VALUE val = read_field(buf, spec->data.map->value);
        rb_hash_aset(hsh, key, val);
      }
      
      read_map_end(buf);
      
      return hsh;
    }
    
    case T_LIST: {
      list_header hdr;
      VALUE arr;
      int i;
      
      read_list_begin(buf, &hdr);
      arr = rb_ary_new2(hdr.num_elements);
      
      for (i = 0; i < hdr.num_elements; ++i) {
        rb_ary_push(arr, read_field(buf, spec->data.element));
      }
      
      read_list_end(buf);
      
      return arr;
    }
    
    case T_SET: {
      VALUE set;
      set_header hdr;
      int i;
      
      read_set_begin(buf, &hdr);
      set = rb_hash_new();
      
      for (i = 0; i < hdr.num_elements; ++i) {
        rb_hash_aset(set, read_field(buf, spec->data.element), Qtrue);
      }
      
      return set;
    }
    
    
    default:
      return read_type(spec->type, buf);
  }
}

VALUE read_struct(VALUE obj, decode_buffer* buf) {
  VALUE field;
  field_header f_header;
  VALUE value = Qnil;
  VALUE fields = rb_const_get(CLASS_OF(obj), fields_id);
  field_spec* spec;
  char name_buf[128];
    
  read_struct_begin(buf);
  
  while(true) {
    read_field_begin(buf, &f_header);
    if (T_STOP == f_header.type) {
      break;
    }
    
    field = rb_hash_aref(fields, INT2FIX(f_header.id));
    
    if (NIL_P(field)) {
      skip_type(f_header.type, buf);
    } 
    else {
      spec = parse_field_spec(field);

      if (spec->type != f_header.type) {
        skip_type(spec->type, buf);
      } else {
        value = read_field(buf, spec);
        name_buf[0] = '@';
        strlcpy(&name_buf[1], spec->name, sizeof(name_buf) - 1);
        
        rb_iv_set(obj, name_buf, value);
      }
      
      free_field_spec(spec);
    }
    
    read_field_end(buf);
  }
  
  read_struct_end(buf);
  
  return obj;
}

VALUE tfbp_decode_binary(VALUE self, VALUE obj, VALUE transport) {
  decode_buffer buf;
  VALUE ret_val;
  
  VALUE str_buf = rb_funcall(transport, string_buffer_id, 0);

  buf.pos = 0;  
  buf.data = RSTRING(str_buf)->ptr;
  buf.len = RSTRING(str_buf)->len;  // TODO(kevinclark): Make sure we don't overrun this.
  buf.trans = transport;       // We need to hold this so the buffer can be refilled

#ifdef __DEBUG__
  rb_p(rb_str_new2("Running decode binary with data:"));
  rb_p(rb_inspect(rb_str_new2(buf.data)));
#endif
 
  ret_val = read_struct(obj, &buf);
  
  // Consume whatever was read
  rb_funcall(buf.trans, consume_bang_id, 1, INT2FIX(buf.pos));
  
  return ret_val;
}

void Init_tfastbinaryprotocol()
{
  VALUE class_tbinproto = rb_const_get(rb_cObject, rb_intern("TBinaryProtocol"));
  class_tfbp = rb_define_class("TFastBinaryProtocol", class_tbinproto);
  type_sym = ID2SYM(rb_intern("type"));
  class_sym = ID2SYM(rb_intern("class"));
  key_sym = ID2SYM(rb_intern("key"));
  value_sym = ID2SYM(rb_intern("value"));
  name_sym = ID2SYM(rb_intern("name"));
  fields_id = rb_intern("FIELDS");
  element_sym = ID2SYM(rb_intern("element"));
  consume_bang_id = rb_intern("consume!");
  string_buffer_id = rb_intern("string_buffer");
  refill_buffer_id = rb_intern("refill_buffer");
  
  // For fast access
  rb_define_method(class_tfbp, "encode_binary", tfbp_encode_binary, 1);
  rb_define_method(class_tfbp, "decode_binary", tfbp_decode_binary, 2);
}
