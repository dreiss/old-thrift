require File.join(File.dirname(__FILE__), '../test_helper')
require File.join(File.dirname(__FILE__), '../fixtures/structs')

require 'thrift/transport/ttransport'
require 'thrift/protocol/tbinaryprotocol'
require 'thrift/protocol/c/tfastbinaryprotocol'

class TFastBinaryProtocolTest < Test::Unit::TestCase
  def test_encodes_and_decodes_bools
    trans = TMemoryBuffer.new
    proto = TFastBinaryProtocol.new(trans)
    obj = Fixtures::Structs::OneBool.new
    assert_equal "\001", proto.encode_binary(obj)
  end
end

# This class tests that the read_* and write_* methods on 
# TFastBinaryProtocol do the same as TProtocol. This should be phased out
# for a class which tests that a thrift struct is properly serialized and
# deserialized
class TFastBinaryProtocolCompatablityTest < Test::Unit::TestCase
  I8_MIN = -128
  I8_MAX = 127
  I16_MIN = -32768
  I16_MAX = 32767
  I32_MIN = -2147483648
  I32_MAX = 2147483647
  I64_MIN = -9223372036854775808
  I64_MAX = 9223372036854775807
  DBL_MIN = Float::MIN
  DBL_MAX = Float::MAX

  # booleans might be read back differently, so we supply a list [write_value, read_value]
  BOOL_VALUES = [[0,true], [14,true], [-14,true], [true,true], [false,false], ["",true], [nil,false]]
  BYTE_VALUES = [14, -14, I8_MIN, I8_MAX]
  I16_VALUES = [400, 0, -234, I16_MIN, I16_MAX]
  I32_VALUES = [325, 0, -1, -1073741825, -278, -4352388, I32_MIN, I32_MAX]
  I64_VALUES = [15, 0, -33, I64_MIN, I64_MAX]
  DBL_VALUES = [DBL_MIN, -33.8755, 0, 3658.1279, DBL_MAX]
  STR_VALUES = ["", "welcome to my test"]

  def setup
    @transport = TMemoryBuffer.new

    @ruby_protocol = TBinaryProtocol.new(@transport)
    @c_protocol = TFastBinaryProtocol.new(@transport)
  end

  def trans_impl(protocol)
    assert_equal @transport, protocol.trans
  end

  def test_trans
    trans_impl @ruby_protocol
    trans_impl @c_protocol
  end

  def byte_impl(writer, reader)
    assert_equal 0, @transport.available

    BYTE_VALUES.each do |v|
      writer.writeByte(v)
      assert_equal 1, @transport.available
      assert_equal v, reader.readByte
      assert_equal 0, @transport.available
    end
  end

  def test_byte
    byte_impl @ruby_protocol, @c_protocol
    byte_impl @ruby_protocol, @c_protocol
    byte_impl @c_protocol, @ruby_protocol
    byte_impl @c_protocol, @c_protocol
  end

  def bool_impl(writer, reader)
    assert_equal 0, @transport.available

    BOOL_VALUES.each do |write,read|
      writer.writeBool(write)
      assert_equal 1, @transport.available
      assert_equal read, reader.readBool
      assert_equal 0, @transport.available
    end
  end

  def test_bool
    bool_impl @ruby_protocol, @ruby_protocol
    bool_impl @ruby_protocol, @c_protocol
    bool_impl @c_protocol, @ruby_protocol
    bool_impl @c_protocol, @c_protocol
  end

  def i16_impl(writer, reader)
    assert_equal 0, @transport.available

    I16_VALUES.each do |v|
      writer.writeI16(v)
      assert_equal 2, @transport.available
      assert_equal v, reader.readI16
      assert_equal 0, @transport.available
    end
  end

  def test_i16
    i16_impl @ruby_protocol, @ruby_protocol
    i16_impl @ruby_protocol, @c_protocol
    i16_impl @c_protocol, @ruby_protocol
    i16_impl @c_protocol, @c_protocol
  end

  def i32_impl(writer, reader)
    assert_equal 0, @transport.available

    I32_VALUES.each do |v|
      writer.writeI32(v)
      assert_equal 4, @transport.available
      assert_equal v, reader.readI32
      assert_equal 0, @transport.available
    end
  end

  def test_i32
    i32_impl @ruby_protocol, @ruby_protocol
    i32_impl @ruby_protocol, @c_protocol
    i32_impl @c_protocol, @ruby_protocol
    i32_impl @c_protocol, @c_protocol
  end

  def i64_impl(writer, reader)
    assert_equal 0, @transport.available

    I64_VALUES.each do |v|
      writer.writeI64(v)
      assert_equal 8, @transport.available
      assert_equal v, reader.readI64
      assert_equal 0, @transport.available
    end
  end

  def test_i64
    i64_impl @ruby_protocol, @ruby_protocol
    i64_impl @ruby_protocol, @c_protocol
    i64_impl @c_protocol, @ruby_protocol
    i64_impl @c_protocol, @c_protocol
  end

  def double_impl(writer, reader)
    assert_equal 0, @transport.available

    DBL_VALUES.each do |v|
      writer.writeDouble(v)
      assert_equal 8, @transport.available
      assert_equal v, reader.readDouble
      assert_equal 0, @transport.available
    end
  end

  def test_double
    double_impl @ruby_protocol, @ruby_protocol
    double_impl @ruby_protocol, @c_protocol
    double_impl @c_protocol, @ruby_protocol
    double_impl @c_protocol, @c_protocol
  end

  def string_impl(writer, reader)
    assert_equal 0, @transport.available

    STR_VALUES.each do |s|
      writer.writeString(s)
      assert_equal 4 + s.size, @transport.available
      assert_equal s, reader.readString
      assert_equal 0, @transport.available
    end
  end

  def test_string
    string_impl @ruby_protocol, @ruby_protocol
    string_impl @ruby_protocol, @c_protocol
    string_impl @c_protocol, @ruby_protocol
    string_impl @c_protocol, @c_protocol
  end

  def type_impl(writer, reader)
    assert_equal 0, @transport.available

    BOOL_VALUES.each do |write,read|
      writer.write_type(TType::BOOL, write)
      assert 1, @transport.available
      assert_equal read, reader.read_type(TType::BOOL)
      assert_equal 0, @transport.available
    end

    BYTE_VALUES.each do |v|
      writer.write_type(TType::BYTE, v)
      assert 1, @transport.available
      assert_equal v, reader.read_type(TType::BYTE)
      assert_equal 0, @transport.available
    end

    I16_VALUES.each do |v|
      writer.write_type(TType::I16, v)
      assert 2, @transport.available
      assert_equal v, reader.read_type(TType::I16)
      assert_equal 0, @transport.available
    end

    I32_VALUES.each do |v|
      writer.write_type(TType::I32, v)
      assert 4, @transport.available
      assert_equal v, reader.read_type(TType::I32)
      assert_equal 0, @transport.available
    end

    I64_VALUES.each do |v|
      writer.write_type(TType::I64, v)
      assert 8, @transport.available
      assert_equal v, reader.read_type(TType::I64)
      assert_equal 0, @transport.available
    end

    DBL_VALUES.each do |v|
      writer.write_type(TType::DOUBLE, v)
      assert 8, @transport.available
      assert_equal v, reader.read_type(TType::DOUBLE)
      assert_equal 0, @transport.available
    end

    STR_VALUES.each do |v|
      writer.write_type(TType::STRING, v)
      assert 4 + v.size, @transport.available
      assert_equal v, reader.read_type(TType::STRING)
      assert_equal 0, @transport.available
    end

    o = Fixtures::Structs::OneOfEach.new
    o.im_true = true;
    o.im_false = false;
    o.a_bite = -42;
    o.integer16 = 27000;
    o.integer32 = 1<<24;
    o.integer64 = 6000 * 1000 * 1000;
    o.double_precision = Math::PI;
    o.some_characters = "Debug THIS!";
    o.zomg_unicode = "\xd7\n\a\t";
    writer.write_type(TType::STRUCT, o)
    ro = Fixtures::Structs::OneOfEach.new
    ro.read(reader)
    assert_equal 0, @transport.available
    assert_equal(o, ro)

    assert_raise(NotImplementedError) { writer.write_type(0, 0) }
    assert_equal 0, @transport.available
  end

  def test_type
    type_impl @ruby_protocol, @ruby_protocol
    type_impl @ruby_protocol, @c_protocol
    type_impl @c_protocol, @ruby_protocol
    type_impl @c_protocol, @c_protocol
  end
end
