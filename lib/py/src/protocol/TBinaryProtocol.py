# Copyright (c) 2006- Facebook
# Distributed under the Thrift Software License
#
# See accompanying file LICENSE or visit the Thrift site at:
# http://developers.facebook.com/thrift/

import md5
from TProtocol import *
from struct import pack, unpack

class TBinaryProtocol(TProtocolBase):

  """Binary implementation of the Thrift protocol driver."""

  # NastyHaxx. Python 2.4+ on 32-bit machines forces hex constants to be
  # positive, converting this into a long. If we hardcode the int value
  # instead it'll stay in 32 bit-land.

  # VERSION_MASK = 0xffff0000
  VERSION_MASK = -65536

  # VERSION_1 = 0x80010000
  VERSION_1 = -2147418112

  TYPE_MASK = 0x000000ff
  MD5LEN = md5.digest_size

  def __init__(self, trans, strictRead=False, strictWrite=True):
    TProtocolBase.__init__(self, trans)
    self.strictRead = strictRead
    self.strictWrite = strictWrite

  def writeMessageBegin(self, name, type, seqid):
    if self.strictWrite:
      self.writeI32(TBinaryProtocol.VERSION_1 | type)
      self.writeString(name)
      self.writeI32(seqid)
    else:
      self.writeString(name)
      self.writeByte(type)
      self.writeI32(seqid)

  def writeMessageEnd(self):
    pass

  def writeStructBegin(self, name):
    pass

  def writeStructEnd(self):
    pass

  def writeFieldBegin(self, name, type, id):
    self.writeByte(type)
    self.writeI16(id)

  def writeFieldEnd(self):
    pass

  def writeFieldStop(self):
    self.writeByte(TType.STOP);

  def writeMapBegin(self, ktype, vtype, size):
    self.writeByte(ktype)
    self.writeByte(vtype)
    self.writeI32(size)

  def writeMapEnd(self):
    pass

  def writeListBegin(self, etype, size):
    self.writeByte(etype)
    self.writeI32(size)

  def writeListEnd(self):
    pass

  def writeSetBegin(self, etype, size):
    self.writeByte(etype)
    self.writeI32(size)

  def writeSetEnd(self):
    pass

  def writeBool(self, bool):
    if bool:
      self.writeByte(1)
    else:
      self.writeByte(0)

  def writeByte(self, byte):
    buff = pack("!b", byte)
    self.trans.write(buff)

  def writeI16(self, i16):
    buff = pack("!h", i16)
    self.trans.write(buff)

  def writeI32(self, i32):
    buff = pack("!i", i32)
    self.trans.write(buff)

  def writeI64(self, i64):
    buff = pack("!q", i64)
    self.trans.write(buff)

  def writeDouble(self, dub):
    buff = pack("!d", dub)
    self.trans.write(buff)

  def writeString(self, str):
    self.writeI32(len(str))
    self.trans.write(str)

  def writeAny(self, any):
    if isinstance(any, bool):
      self.writeByte(TType.BOOL)
      self.writeBool(any)
    elif isinstance(any, int):
      self.writeByte(TType.I32)
      self.writeI32(any)
    elif isinstance(any, long):
      self.writeByte(TType.I64)
      self.writeI64(any)
    elif isinstance(any, str):
      self.writeByte(TType.STRING)
      self.writeString(any)
    elif isinstance(any, float):
      self.writeByte(TType.DOUBLE)
      self.writeDouble(any)
    elif hasattr(any, 'binary_fingerprint'):
      assert hasattr(any, 'write')
      self.writeByte(TType.STRUCT)
      self.trans.write(any.binary_fingerprint)
      any.write(self)
    else:
      raise Exception, "any can't handle type '%s'" % repr(type(any))

  def readMessageBegin(self):
    sz = self.readI32()
    if sz < 0:
      version = sz & TBinaryProtocol.VERSION_MASK
      if version != TBinaryProtocol.VERSION_1:
        raise TProtocolException(TProtocolException.BAD_VERSION, 'Bad version in readMessageBegin: %d' % (sz))
      type = sz & TBinaryProtocol.TYPE_MASK
      name = self.readString()
      seqid = self.readI32()
    else:
      if self.strictRead:
        raise TProtocolException(TProtocolException.BAD_VERSION, 'No protocol version header')
      name = self.trans.readAll(sz)
      type = self.readByte()
      seqid = self.readI32()
    return (name, type, seqid)

  def readMessageEnd(self):
    pass

  def readStructBegin(self):
    pass

  def readStructEnd(self):
    pass

  def readFieldBegin(self):
    type = self.readByte()
    if type == TType.STOP:
      return (None, type, 0)
    id = self.readI16()
    return (None, type, id)

  def readFieldEnd(self):
    pass

  def readMapBegin(self):
    ktype = self.readByte()
    vtype = self.readByte()
    size = self.readI32()
    return (ktype, vtype, size)

  def readMapEnd(self):
    pass

  def readListBegin(self):
    etype = self.readByte()
    size = self.readI32()
    return (etype, size)

  def readListEnd(self):
    pass

  def readSetBegin(self):
    etype = self.readByte()
    size = self.readI32()
    return (etype, size)

  def readSetEnd(self):
    pass

  def readBool(self):
    byte = self.readByte()
    if byte == 0:
      return False
    return True

  def readByte(self):
    buff = self.trans.readAll(1)
    val, = unpack('!b', buff)
    return val

  def readI16(self):
    buff = self.trans.readAll(2)
    val, = unpack('!h', buff)
    return val

  def readI32(self):
    buff = self.trans.readAll(4)
    val, = unpack('!i', buff)
    return val

  def readI64(self):
    buff = self.trans.readAll(8)
    val, = unpack('!q', buff)
    return val

  def readDouble(self):
    buff = self.trans.readAll(8)
    val, = unpack('!d', buff)
    return val

  def readAny(self):
    type, = unpack('!b', self.trans.readAll(1))
    if type == TType.BYTE:
      return self.readByte()
    if type == TType.BYTE:
      return self.readByte()
    if type == TType.I16:
      return self.readI16()
    if type == TType.I32:
      return self.readI32()
    if type == TType.I64:
      return self.readI64()
    if type == TType.DOUBLE:
      return self.readDouble()
    if type == TType.STRING:
      return self.readString()
    if type == TType.STRUCT:
      finger = self.trans.read(TBinaryProtocol.MD5LEN)
      class_ = StructFactory.get(finger)
      if class_:
	result = class_()
	result.read(self)
	return result
      else:
	self.skip(TType.STRUCT)
	return
    raise Exception, "can't guess type of '%d'" % type

  def readString(self):
    len = self.readI32()
    str = self.trans.readAll(len)
    return str


class TBinaryProtocolFactory:
  def __init__(self, strictRead=False, strictWrite=True):
    self.strictRead = strictRead
    self.strictWrite = strictWrite

  def getProtocol(self, trans):
    prot = TBinaryProtocol(trans, self.strictRead, self.strictWrite)
    return prot


class TBinaryProtocolAccelerated(TBinaryProtocol):

  """C-Accelerated version of TBinaryProtocol.

  This class does not override any of TBinaryProtocol's methods,
  but the generated code recognizes it directly and will call into
  our C module to do the encoding, bypassing this object entirely.
  We inherit from TBinaryProtocol so that the normal TBinaryProtocol
  encoding can happen if the fastbinary module doesn't work for some
  reason.  (TODO(dreiss): Make this happen sanely in more cases.)

  In order to take advantage of the C module, just use
  TBinaryProtocolAccelerated instead of TBinaryProtocol.

  NOTE:  This code was contributed by an external developer.
         The internal Thrift team has reviewed and tested it,
         but we cannot guarantee that it is production-ready.
         Please feel free to report bugs and/or success stories
         to the public mailing list.
  """

  pass


class TBinaryProtocolAcceleratedFactory:
  def getProtocol(self, trans):
    return TBinaryProtocolAccelerated(trans)
