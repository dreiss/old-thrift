#include <stdint.h>

#include <ruby.h>


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


static VALUE class_tfbp;
static ID id_write;
static ID id_read_all;
static ID id_trans;

static const uint32_t VERSION_MASK = 0xffff0000;
static const uint32_t VERSION_1 = 0x80010000;


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
