// Half of this file comes from contributions from Nitay Joffe (nitay@powerset.com)
// Much of the rest (almost) directly ported (or pulled) from fastbinary.c
#include <stdint.h>

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
static ID id_write;
static ID id_read_all;
static ID id_trans;

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
  VALUE klass;
  struct _thrift_map* map;
  struct _field_spec* element;
} container_data;

typedef struct _field_spec{
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
  int type = NUM2INT(rb_hash_aref(field_data, ID2SYM(rb_intern("type"))));
  VALUE name = rb_hash_aref(field_data, ID2SYM(rb_intern("name")));
  
  field_spec* spec = (field_spec *) malloc(sizeof(field_spec));
  bzero(spec, sizeof(field_spec));
  
  spec->type = type;
  
  if (Qnil != name) {
    spec->name = STR2CSTR(name);
  } else {
    spec->name = NULL;
  }
  
  switch(type) {
    case T_STRCT: {
      spec->data.klass = rb_hash_aref(field_data, ID2SYM(rb_intern("class")));
      break;
    }
    
    case T_MAP: {
      VALUE key_fields = rb_hash_aref(field_data, ID2SYM(rb_intern("key")));
      VALUE value_fields = rb_hash_aref(field_data, ID2SYM(rb_intern("value")));
      field_spec* key = parse_field_spec(key_fields);;
      field_spec* val = parse_field_spec(value_fields);;
      thrift_map* map;
      
      map = (thrift_map *) malloc(sizeof(thrift_map));
      
      map->key = key;
      map->value = val;
      spec->data.map = map;
      
      break;
    }
    
    case T_LIST: 
    case T_SET:
    {
      VALUE list_fields = rb_hash_aref(field_data, ID2SYM(rb_intern("element")));
      field_spec* element = parse_field_spec(list_fields);
      spec->data.element = element;
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
  VALUE value = rb_ivar_get(obj, rb_intern(STR2CSTR(rb_str_concat(rb_str_new2("@"), rb_str_new2(spec->name)))));

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

// TODO(kevinclark): I'm using NUM2INT here in testing because it essentially does a type assert
// We should go back and test how much of a speedup we get by using FIX2INT (and friends) instead
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
      write_string(buf, STR2CSTR(obj));
      break;
          
    case T_STRCT: {
      // FIELDS = {
      //   1 => {:type => TType::BOOL, :name => 'im_true'},
      //   2 => {:type => TType::BOOL, :name => 'im_false'},
      //   3 => {:type => TType::BYTE, :name => 'a_bite'},
      //   4 => {:type => TType::I16, :name => 'integer16'},
      //   5 => {:type => TType::I32, :name => 'integer32'}
      // }
      
      VALUE args = rb_ary_new3(2, buf, obj);
      VALUE fields = rb_const_get(CLASS_OF(obj), rb_intern("FIELDS"));
      
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
// TFastBinaryProtocol read functions
// -----------------------------------------------------------------------------

/*
 * def readByte()
 *   dat = trans.readAll(1)
 *   val = dat[0]
 *   if (val > 0x7f)
 *     val = 0 - ((val - 1) ^ 0xff)
 *   end
 *   return val
 * end
 */
int8_t tfbp_read_byte_internal(VALUE self)
{
  VALUE transport = rb_ivar_get(self, id_trans);
  VALUE data = rb_funcall(transport, id_read_all, 1, INT2FIX(1));

  return StringValuePtr(data)[0];
}

VALUE tfbp_read_byte(VALUE self)
{
  return INT2FIX(tfbp_read_byte_internal(self));
}


/*
 * def readBool()
 *   byte = readByte()
 *   return byte != 0
 * end
 */
VALUE tfbp_read_bool(VALUE self)
{
  int8_t byte = tfbp_read_byte_internal(self);
  return byte != 0 ? Qtrue : Qfalse;
}


/*
 * def readI16()
 *   dat = trans.readAll(2)
 *   val, = dat.unpack('n')
 *   if (val > 0x7fff)
 *     val = 0 - ((val - 1) ^ 0xffff)
 *   end
 *   return val
 * end
 */
VALUE tfbp_read_i16(VALUE self)
{
  VALUE transport = rb_ivar_get(self, id_trans);
  VALUE data = rb_funcall(transport, id_read_all, 1, INT2FIX(2));

  char *d = StringValuePtr(data);
  int16_t val = *(int16_t *)d;
  val = ntohs(val);
  return INT2FIX(val);
}


/*
 * def readI32()
 *   dat = trans.readAll(4)
 *   val, = dat.unpack('N')
 *   if (val > 0x7fffffff)
 *     val = 0 - ((val - 1) ^ 0xffffffff)
 *   end
 *   return val
 * end
 */
int32_t tfbp_read_i32_internal(VALUE self)
{
  VALUE transport = rb_ivar_get(self, id_trans);
  VALUE data = rb_funcall(transport, id_read_all, 1, INT2FIX(4));

  char *d = RSTRING(data)->ptr;
  int32_t val = *(int32_t *)d;
  return ntohl(val);
}

VALUE tfbp_read_i32(VALUE self)
{
  return INT2NUM(tfbp_read_i32_internal(self));
}


/*
 * def readI64()
 *   dat = trans.readAll(8)
 *   hi, lo = dat.unpack('N2')
 *   if (hi > 0x7fffffff)
 *     hi = hi ^ 0xffffffff
 *     lo = lo ^ 0xffffffff
 *     return 0 - hi*4294967296 - lo - 1
 *   else
 *     return hi*4294967296 + lo
 *   end
 * end
 */
VALUE tfbp_read_i64(VALUE self)
{
  VALUE transport = rb_ivar_get(self, id_trans);
  VALUE data = rb_funcall(transport, id_read_all, 1, INT2FIX(8));

  char *d = RSTRING(data)->ptr;
  int32_t hi = *(int32_t *)d;
  d += sizeof(int32_t);
  uint32_t lo = *(uint32_t *)d;

  hi = ntohl(hi);
  lo = ntohl(lo);

  int64_t val;
  if (hi & 0x80000000) {
    hi = ~hi;
    lo = ~lo;
    val = 0 - ((int64_t)hi << 32) - lo - 1;
  }
  else {
    val = ((int64_t)hi << 32) + (uint64_t)lo;
  }

  return rb_ll2inum(val);
}


/*
 * def readDouble()
 *   dat = trans.readAll(8)
 *   val, = dat.unpack('G')
 *   return val
 * end
 */
VALUE tfbp_read_double(VALUE self)
{
  VALUE transport = rb_ivar_get(self, id_trans);
  VALUE data = rb_funcall(transport, id_read_all, 1, INT2FIX(8));

  VALUE val_array = rb_funcall(data, rb_intern("unpack"), 1, rb_str_new("G", 1));
  VALUE val = rb_ary_entry(val_array, 0);
  return val;
}


/*
 * def readString()
 *   sz = readI32()
 *   dat = trans.readAll(sz)
 *   return dat
 * end
 */
VALUE tfbp_read_string(VALUE self)
{
  int32_t size = tfbp_read_i32_internal(self);
  VALUE transport = rb_ivar_get(self, id_trans);
  VALUE data = rb_funcall(transport, id_read_all, 1, INT2FIX(size));
  return data;
}


/*
 * def readStructBegin(); nil; end
 */
VALUE tfbp_read_struct_begin(VALUE self)
{
  return Qnil;
}


/*
 * def readStructEnd(); nil; end
 */
VALUE tfbp_read_struct_end(VALUE self)
{
  return Qnil;
}


/*
 * def readListBegin()
 *   etype = readByte()
 *   size = readI32()
 *   return etype, size
 * end
 */
void tfbp_read_list_begin_internal(VALUE self, int8_t *etype, int32_t *size)
{
  *etype = tfbp_read_byte_internal(self);
  *size = tfbp_read_i32_internal(self);
}

VALUE tfbp_read_list_begin(VALUE self)
{
  int8_t etype;
  int32_t size;
  tfbp_read_list_begin_internal(self, &etype, &size);
  return rb_ary_new3(2, INT2FIX(etype), INT2NUM(size));
}


/*
 * def readListEnd(); nil; end
 */
VALUE tfbp_read_list_end(VALUE self)
{
  return Qnil;
}


/*
 * def readSetBegin()
 *   etype = readByte()
 *   size = readI32()
 *   return etype, size
 * end
 */
void tfbp_read_set_begin_internal(VALUE self, int8_t *etype, int32_t *size)
{
  *etype = tfbp_read_byte_internal(self);
  *size = tfbp_read_i32_internal(self);
}

VALUE tfbp_read_set_begin(VALUE self)
{
  int8_t etype;
  int32_t size;
  tfbp_read_set_begin_internal(self, &etype, &size);
  return rb_ary_new3(2, INT2FIX(etype), INT2NUM(size));
}


/*
 * def readSetEnd(); nil; end
 */
VALUE tfbp_read_set_end(VALUE self)
{
  return Qnil;
}


/*
 * def readMessageBegin()
 *   version = readI32()
 *   if (version & VERSION_MASK != VERSION_1)
 *     raise TProtocolException.new(TProtocolException::BAD_VERSION, 'Missing version identifier')
 *   end
 *   type = version & 0x000000ff
 *   name = readString()
 *   seqid = readI32()
 *   return name, type, seqid
 * end
 */
VALUE tfbp_read_message_begin(VALUE self)
{
  int32_t version = tfbp_read_i32_internal(self);
  if ((version & VERSION_MASK) != VERSION_1) {
    ID tprotocol_exception = rb_intern("TProtocolException");
    VALUE exception = rb_funcall(tprotocol_exception, rb_intern("new"), 2, rb_const_get(tprotocol_exception, rb_intern("BAD_VERSION")), rb_str_new2("Missing version identifier"));
    rb_raise(exception, "");
  }
  int32_t type = version & 0x000000ff;
  VALUE name = tfbp_read_string(self);
  VALUE seqid = tfbp_read_i32(self);
  return rb_ary_new3(3, name, INT2NUM(type), seqid);
}


/*
 * def readMessageEnd(); nil; end
 */
VALUE tfbp_read_message_end(VALUE self)
{
  return Qnil;
}


/*
 * def readFieldBegin()
 *   type = readByte()
 *   if (type === TType::STOP)
 *     return nil, type, 0
 *   end
 *   id = readI16()
 *   return nil, type, id
 * end
 */
void tfbp_read_field_begin_internal(VALUE self, int8_t *type, VALUE *id)
{
  *type = tfbp_read_byte_internal(self);
  if (*type == T_STOP) {
    *id = 0;
  }
  else {
    *id = tfbp_read_i16(self);
  }
}

VALUE tfbp_read_field_begin(VALUE self)
{
  int8_t type;
  VALUE id;
  tfbp_read_field_begin_internal(self, &type, &id);
  return rb_ary_new3(3, Qnil, INT2FIX(type), id);
}


/*
 * def readFieldEnd(); nil; end
 */
VALUE tfbp_read_field_end(VALUE self)
{
  return Qnil;
}


/*
 * def readMapBegin()
 *   ktype = readByte()
 *   vtype = readByte()
 *   size = readI32()
 *   return ktype, vtype, size
 * end
 */
void tfbp_read_map_begin_internal(VALUE self, int8_t *ktype, int8_t *vtype, int32_t *size)
{
  *ktype = tfbp_read_byte_internal(self);
  *vtype = tfbp_read_byte_internal(self);
  *size = tfbp_read_i32_internal(self);
}

VALUE tfbp_read_map_begin(VALUE self)
{
  int8_t ktype;
  int8_t vtype;
  int32_t size;
  tfbp_read_map_begin_internal(self, &ktype, &vtype, &size);
  return rb_ary_new3(3, INT2FIX(ktype), INT2FIX(vtype), INT2NUM(size));
}


/*
 * def readMapEnd(); nil; end
 */
VALUE tfbp_read_map_end(VALUE self)
{
  return Qnil;
}


/*
 * def read_type(type)
 *   case type
 *   when TType::BOOL
 *     readBool
 *   when TType::BYTE
 *     readByte
 *   when TType::DOUBLE
 *     readDouble
 *   when TType::I16
 *     readI16
 *   when TType::I32
 *     readI32
 *   when TType::I64
 *     readI64
 *   when TType::STRING
 *     readString
 *   else
 *     raise NotImplementedError
 *   end
 * end
 */
VALUE tfbp_read_type(VALUE self, VALUE type)
{
  uint32_t type_value = NUM2INT(type);
  switch (type_value) {
    case T_BOOL:
      return tfbp_read_bool(self);
    case T_BYTE:
      return tfbp_read_byte(self);
    case T_I16:
      return tfbp_read_i16(self);
    case T_I32:
      return tfbp_read_i32(self);
    case T_I64:
      return tfbp_read_i64(self);
    case T_DBL:
      return tfbp_read_double(self);
    case T_STR:
      return tfbp_read_string(self);
    default:
      rb_raise(rb_eNotImpError, "");
      break;
  }
  return self;
}


// -----------------------------------------------------------------------------
// TFastBinaryProtocol write functions
// -----------------------------------------------------------------------------

/*
 * def writeByte(byte)
 *   trans.write([byte].pack('n')[1..1])
 * end
 */
VALUE tfbp_write_byte(VALUE self, VALUE val)
{
  int16_t value = NUM2INT(val);
  value = htons(value);
  char *d = (char *)&value;
  ++d;
  VALUE str = rb_str_new(d, 1);

  VALUE transport = rb_ivar_get(self, id_trans);
  rb_funcall(transport, id_write, 1, str);
  return Qnil;
}


/*
 * def writeBool(bool)
 *   if (bool)
 *     writeByte(1)
 *   else
 *     writeByte(0)
 *   end
 * end
 */
VALUE tfbp_write_bool(VALUE self, VALUE val)
{
  switch (TYPE(val)) {
    case T_FALSE:
    case T_NIL:
      tfbp_write_byte(self, INT2FIX(0));
      break;
    default:
      tfbp_write_byte(self, INT2FIX(1));
      break;
  }
  return Qnil;
}


/*
 * def writeI16(i16)
 *   trans.write([i16].pack('n'))
 * end
 */
VALUE tfbp_write_i16(VALUE self, VALUE val)
{
  int16_t value = NUM2INT(val);
  value = htons(value);
  VALUE str = rb_str_new((char *)&value, sizeof(value));

  VALUE transport = rb_ivar_get(self, id_trans);
  rb_funcall(transport, id_write, 1, str);
  return Qnil;
}


/*
 * def writeI32(i32)
 *   trans.write([i32].pack('N'))
 * end
 */
VALUE tfbp_write_i32(VALUE self, VALUE val)
{
  int32_t value = NUM2INT(val);
  value = htonl(value);
  VALUE str = rb_str_new((char *)&value, sizeof(value));

  VALUE transport = rb_ivar_get(self, id_trans);
  rb_funcall(transport, id_write, 1, str);
  return Qnil;
}


/*
 * def writeI64(i64)
 *   hi = i64 >> 32
 *   lo = i64 & 0xffffffff
 *   trans.write([hi, lo].pack('N2'))
 * end
 */
VALUE tfbp_write_i64(VALUE self, VALUE data)
{
  char buf[sizeof(int64_t)];
  int32_t *hi = (int32_t *)buf;
  int32_t *lo = (int32_t *)(buf + sizeof(int32_t));

  int64_t val;
  switch (TYPE(data)) {
    case T_FIXNUM:
      val = NUM2INT(data);
      break;
    case T_BIGNUM:
      val = rb_num2ll(data);
      break;
    default:
      rb_raise(rb_eArgError, "");
  }

  *hi = val >> 32;
  *lo = val & UINT32_MAX;

  *hi = htonl(*hi);
  *lo = htonl(*lo);

  VALUE str = rb_str_new(buf, sizeof(buf));
  VALUE transport = rb_ivar_get(self, id_trans);
  rb_funcall(transport, id_write, 1, str);

  return Qnil;
}


/*
 * def writeDouble(dub)
 *   trans.write([dub].pack('G'))
 * end
 */
VALUE tfbp_write_double(VALUE self, VALUE dub)
{
  VALUE dub_array = rb_ary_new3(1, dub);
  VALUE str = rb_funcall(dub_array, rb_intern("pack"), 1, rb_str_new("G", 1));

  VALUE transport = rb_ivar_get(self, id_trans);
  rb_funcall(transport, id_write, 1, str);
  return Qnil;
}


/*
 * def writeString(str)
 *   writeI32(str.length)
 *   trans.write(str)
 * end
 */
VALUE tfbp_write_string(VALUE self, VALUE str)
{
  tfbp_write_i32(self, INT2FIX(RSTRING(str)->len));
  VALUE transport = rb_ivar_get(self, id_trans);
  rb_funcall(transport, id_write, 1, str);
  return Qnil;
}


/*
 * def writeMessageBegin(name, type, seqid)
 *   writeI32(VERSION_1 | type)
 *   writeString(name)
 *   writeI32(seqid)
 * end
 */
VALUE tfbp_write_message_begin(VALUE self, VALUE name, VALUE type, VALUE seqid)
{
  int32_t type_val = NUM2INT(type);
  tfbp_write_i32(self, INT2FIX(VERSION_1 | type_val));
  tfbp_write_string(self, name);
  tfbp_write_i32(self, seqid);
  return Qnil;
}


/*
 * def writeMessageEnd; nil; end
 */
VALUE tfbp_write_message_end(VALUE self)
{
  return Qnil;
}


/*
 * def writeStructBegin(name); nil; end
 */
VALUE tfbp_write_struct_begin(VALUE self, VALUE name)
{
  return Qnil;
}


/*
 * def writeStructEnd(); nil; end
 */
VALUE tfbp_write_struct_end(VALUE self)
{
  return Qnil;
}


/*
 * def writeFieldBegin(name, type, id)
 *   writeByte(type)
 *   writeI16(id)
 * end
 */
VALUE tfbp_write_field_begin(VALUE self, VALUE name, VALUE type, VALUE id)
{
  tfbp_write_byte(self, type);
  tfbp_write_i16(self, id);
  return Qnil;
}


/*
 * def writeFieldEnd(); nil; end
 */
VALUE tfbp_write_field_end(VALUE self)
{
  return Qnil;
}


/*
 * def writeFieldStop()
 *   writeByte(TType::STOP)
 * end
 */
VALUE tfbp_write_field_stop(VALUE self)
{
  tfbp_write_byte(self, INT2FIX(T_STOP));
  return Qnil;
}


/*
 * def writeMapBegin(ktype, vtype, size)
 *   writeByte(ktype)
 *   writeByte(vtype)
 *   writeI32(size)
 * end
 */
VALUE tfbp_write_map_begin(VALUE self, VALUE ktype, VALUE vtype, VALUE size)
{
  tfbp_write_byte(self, ktype);
  tfbp_write_byte(self, vtype);
  tfbp_write_i32(self, size);
  return Qnil;
}


/*
 * def writeMapEnd(); nil; end
 */
VALUE tfbp_write_map_end(VALUE self)
{
  return Qnil;
}


/*
 * def writeListBegin(etype, size)
 *   writeByte(etype)
 *   writeI32(size)
 * end
 */
VALUE tfbp_write_list_begin(VALUE self, VALUE etype, VALUE size)
{
  tfbp_write_byte(self, etype);
  tfbp_write_i32(self, size);
  return Qnil;
}


/*
 * def writeListEnd(); nil; end
 */
VALUE tfbp_write_list_end(VALUE self)
{
  return Qnil;
}


/*
 * def writeSetBegin(etype, size)
 *   writeByte(etype)
 *   writeI32(size)
 * end
 */
VALUE tfbp_write_set_begin(VALUE self, VALUE etype, VALUE size)
{
  tfbp_write_byte(self, etype);
  tfbp_write_i32(self, size);
  return Qnil;
}


/*
 * def writeSetEnd(); nil; end
 */
VALUE tfbp_write_set_end(VALUE self)
{
  return Qnil;
}


/*
 * def write_type(type, value)
 *   case type
 *   when TType::BOOL
 *     writeBool(value)
 *   when TType::BYTE
 *     writeByte(value)
 *   when TType::DOUBLE
 *     writeDouble(value)
 *   when TType::I16
 *     writeI16(value)
 *   when TType::I32
 *     writeI32(value)
 *   when TType::I64
 *     writeI64(value)
 *   when TType::STRING
 *     writeString(value)
 *   when TType::STRUCT
 *     value.write(self)
 *   else
 *     raise NotImplementedError
 *   end
 * end
 */
VALUE tfbp_write_type(VALUE self, VALUE type, VALUE value)
{
  uint32_t type_value = NUM2INT(type);
  switch (type_value) {
    case T_BOOL:
      tfbp_write_bool(self, value);
      break;
    case T_BYTE:
      tfbp_write_byte(self, value);
      break;
    case T_I16:
      tfbp_write_i16(self, value);
      break;
    case T_I32:
      tfbp_write_i32(self, value);
      break;
    case T_I64:
      tfbp_write_i64(self, value);
      break;
    case T_DBL:
      tfbp_write_double(self, value);
      break;
    case T_STR:
      tfbp_write_string(self, value);
      break;
    case T_STRCT:
      rb_funcall(value, id_write, 1, self);
      break;
    default:
      rb_raise(rb_eNotImpError, "");
      break;
  }
  return Qnil;
}


/*
 * def write_field(name, type, fid, value)
 *   writeFieldBegin(name, type, fid)
 *   write_type(type, value)
 *   writeFieldEnd
 * end
 */
VALUE tfbp_write_field(VALUE self, VALUE name, VALUE type, VALUE fid, VALUE value)
{
  tfbp_write_field_begin(self, name, type, fid);
  tfbp_write_type(self, type, value);
  tfbp_write_field_end(self);
  return self;
}


// -----------------------------------------------------------------------------
// TFastBinaryProtocol other functions
// -----------------------------------------------------------------------------

/*
 * def initialize(trans)
 *   super(trans)
 * end
 */
VALUE tfbp_new(VALUE class, VALUE trans)
{
  VALUE obj = rb_class_new_instance(0, NULL, class);
  rb_ivar_set(obj, id_trans, trans);
  return obj;
}


/*
 * attr_reader :trans
 */
VALUE tfbp_trans(VALUE self)
{
  return rb_ivar_get(self, id_trans);
}


/*
 * def skip(type)
 *   if type === TType::STOP
 *     nil
 *   elsif type === TType::BOOL
 *     readBool()
 *   elsif type === TType::BYTE
 *     readByte()
 *   elsif type === TType::I16
 *     readI16()
 *   elsif type === TType::I32
 *     readI32()
 *   elsif type === TType::I64
 *     readI64()
 *   elsif type === TType::DOUBLE
 *     readDouble()
 *   elsif type === TType::STRING
 *     readString()
 *   elsif type === TType::STRUCT
 *     readStructBegin()
 *     while true
 *       name, type, id = readFieldBegin()
 *       if type === TType::STOP
 *         break
 *       else
 *         skip(type)
 *         readFieldEnd()
 *       end
 *       readStructEnd()
 *     end
 *   elsif type === TType::MAP
 *     ktype, vtype, size = readMapBegin()
 *     for i in 1..size
 *       skip(ktype)
 *       skip(vtype)
 *     end
 *     readMapEnd()
 *   elsif type === TType::SET
 *     etype, size = readSetBegin()
 *     for i in 1..size
 *       skip(etype)
 *     end
 *     readSetEnd()
 *   elsif type === TType::LIST
 *     etype, size = readListBegin()
 *     for i in 1..size
 *       skip(etype)
 *     end
 *     readListEnd()
 *   end
 * end
 */
VALUE tfbp_skip_internal(VALUE self, int8_t type)
{
  switch (type) {
    case T_STOP:
      break;
    case T_BOOL:
      tfbp_read_bool(self);
      break;
    case T_BYTE:
      tfbp_read_byte(self);
      break;
    case T_I16:
      tfbp_read_i16(self);
      break;
    case T_I32:
      tfbp_read_i32(self);
      break;
    case T_I64:
      tfbp_read_i64(self);
      break;
    case T_DBL:
      tfbp_read_double(self);
      break;
    case T_STR:
      tfbp_read_string(self);
      break;
    case T_STRCT:
      tfbp_read_struct_begin(self);
      while (1) {
        int8_t type;
        VALUE id;
        tfbp_read_field_begin_internal(self, &type, &id);
        if (type == T_STOP) {
          break;
        }
        else {
          tfbp_skip_internal(self, type);
          tfbp_read_field_end(self);
        }
        tfbp_read_struct_end(self);
      }
    case T_MAP: {
      int8_t ktype;
      int8_t vtype;
      int32_t size;
      tfbp_read_map_begin_internal(self, &ktype, &vtype, &size);
      int32_t i;
      for (i = 0; i < size; ++i) {
        tfbp_skip_internal(self, ktype);
        tfbp_skip_internal(self, vtype);
      }
      tfbp_read_map_end(self);
    }
    case T_SET: {
      int8_t etype;
      int32_t size;
      tfbp_read_set_begin_internal(self, &etype, &size);
      int32_t i;
      for (i = 0; i < size; ++i) {
        tfbp_skip_internal(self, etype);
      }
      tfbp_read_set_end(self);
    }
    case T_LIST: {
      int8_t etype;
      int32_t size;
      tfbp_read_list_begin_internal(self, &etype, &size);
      int32_t i;
      for (i = 0; i < size; ++i) {
        tfbp_skip_internal(self, etype);
      }
      tfbp_read_list_end(self);
    }
  }
  return Qnil;
}

VALUE tfbp_skip(VALUE self, VALUE type)
{
  int8_t type_value = NUM2INT(type);
  return tfbp_skip_internal(self, type_value);
}


void Init_tfastbinaryprotocol()
{
  class_tfbp = rb_define_class("TFastBinaryProtocol", rb_cObject);
  
  id_write = rb_intern("write");
  id_read_all = rb_intern("readAll");
  id_trans = rb_intern("@trans");

  rb_define_singleton_method(class_tfbp, "new", tfbp_new, 1);

  // For fast access
  rb_define_method(class_tfbp, "encode_binary", tfbp_encode_binary, 1);

  rb_define_method(class_tfbp, "skip", tfbp_skip, 1);
  rb_define_method(class_tfbp, "trans", tfbp_trans, 0);

  rb_define_method(class_tfbp, "read_type", tfbp_read_type, 1);
  rb_define_method(class_tfbp, "readStructBegin", tfbp_read_struct_begin, 0);
  rb_define_method(class_tfbp, "readStructEnd", tfbp_read_struct_end, 0);
  rb_define_method(class_tfbp, "readMessageBegin", tfbp_read_message_begin, 0);
  rb_define_method(class_tfbp, "readMessageEnd", tfbp_read_message_end, 0);
  rb_define_method(class_tfbp, "readFieldBegin", tfbp_read_field_begin, 0);
  rb_define_method(class_tfbp, "readFieldEnd", tfbp_read_field_end, 0);
  rb_define_method(class_tfbp, "readMapBegin", tfbp_read_map_begin, 0);
  rb_define_method(class_tfbp, "readMapEnd", tfbp_read_map_end, 0);
  rb_define_method(class_tfbp, "readListBegin", tfbp_read_list_begin, 0);
  rb_define_method(class_tfbp, "readListEnd", tfbp_read_list_end, 0);
  rb_define_method(class_tfbp, "readSetBegin", tfbp_read_set_begin, 0);
  rb_define_method(class_tfbp, "readSetEnd", tfbp_read_set_end, 0);
  rb_define_method(class_tfbp, "readBool", tfbp_read_bool, 0);
  rb_define_method(class_tfbp, "readByte", tfbp_read_byte, 0);
  rb_define_method(class_tfbp, "readI16", tfbp_read_i16, 0);
  rb_define_method(class_tfbp, "readI32", tfbp_read_i32, 0);
  rb_define_method(class_tfbp, "readI64", tfbp_read_i64, 0);
  rb_define_method(class_tfbp, "readDouble", tfbp_read_double, 0);
  rb_define_method(class_tfbp, "readString", tfbp_read_string, 0);

  rb_define_method(class_tfbp, "write_type", tfbp_write_type, 2);
  rb_define_method(class_tfbp, "write_field", tfbp_write_field, 4);
  rb_define_method(class_tfbp, "writeMessageBegin", tfbp_write_message_begin, 3);
  rb_define_method(class_tfbp, "writeMessageEnd", tfbp_write_message_end, 0);
  rb_define_method(class_tfbp, "writeStructBegin", tfbp_write_struct_begin, 1);
  rb_define_method(class_tfbp, "writeStructEnd", tfbp_write_struct_end, 0);
  rb_define_method(class_tfbp, "writeFieldBegin", tfbp_write_field_begin, 3);
  rb_define_method(class_tfbp, "writeFieldEnd", tfbp_write_field_end, 0);
  rb_define_method(class_tfbp, "writeFieldStop", tfbp_write_field_stop, 0);
  rb_define_method(class_tfbp, "writeMapBegin", tfbp_write_map_begin, 3);
  rb_define_method(class_tfbp, "writeMapEnd", tfbp_write_map_end, 0);
  rb_define_method(class_tfbp, "writeListBegin", tfbp_write_list_begin, 2);
  rb_define_method(class_tfbp, "writeListEnd", tfbp_write_list_end, 0);
  rb_define_method(class_tfbp, "writeSetBegin", tfbp_write_set_begin, 2);
  rb_define_method(class_tfbp, "writeSetEnd", tfbp_write_set_end, 0);
  rb_define_method(class_tfbp, "writeBool", tfbp_write_bool, 1);
  rb_define_method(class_tfbp, "writeByte", tfbp_write_byte, 1);
  rb_define_method(class_tfbp, "writeI16", tfbp_write_i16, 1);
  rb_define_method(class_tfbp, "writeI32", tfbp_write_i32, 1);
  rb_define_method(class_tfbp, "writeI64", tfbp_write_i64, 1);
  rb_define_method(class_tfbp, "writeDouble", tfbp_write_double, 1);
  rb_define_method(class_tfbp, "writeString", tfbp_write_string, 1);
}
