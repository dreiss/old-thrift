from TProtocol import *
from struct import pack, unpack

__all__ = ['TCompactProtocol']

CLEAR = 0
BOOL_WRITE = 1
VALUE_WRITE = 2
BOOL_READ = 3
VALUE_READ = 4

def writer(func):
  def nested(self, *args, **kwargs):
    assert self.__state == VALUE_WRITE
    try:
      return func(self, *args, **kwargs)
    finally:
      self.__state = CLEAR
  return nested

def reader(func):
  def nested(self, *args, **kwargs):
    assert self.__state == VALUE_READ
    try:
      return func(self, *args, **kwargs)
    finally:
      self.__state = CLEAR
  return nested

def makeZigZag(n, bits):
  return (n << 1) ^ (n >> (bits - 1))

def fromZigZag(n, bits):
  return (n >> 1) ^ -(n & 1)

TYPES = {True: 0x01, False: 0x02}
class TCompactProtocol(TProtocolBase):
  "Compact implementation of the Thrift protocol driver."

  __state = CLEAR
  __last = None
  __id = None
  def __init__(self, trans):
    TProtocolBase.__init__(self, trans)

  def __writeFieldHeader(self, type, id):
    try:
      if self.__last and id > self.__last:
        delta = id - self.__last
        if delta < 16:
          return self.writeByte(delta << 4 | type)
      self.__writeByte(type)
      self.__writeI16(id)
      return 3
    finally:
      self.__last = id

  def writeFieldBegin(self, name, type, id):
    assert state == CLEAR
    if type == TType.BOOL:
      self.__state = BOOL_WRITE
      self.__id = id
    else:
      self.__state = VALUE_WRITE
      self.__writeFieldHeader(type, id)

  def __writeByte(self, byte):
    self.trans.write(pack('!b', byte))

  def __writeI16(self, i16):
    self.writeVarint(makeZigZag(i16, 16))

  def __writeVarint(self, n):
    out = []
    while True:
      if n & 0x80:
        out.append(n)
        break
      else:
        out.append(n & 0xff)
        n = n >> 7
    self.trans.write(''.join(map(chr, out)))

  def __writeSize(self, i32):
    if i32 > 0x7fff:
      raise TException("thrift can't handle strings longer 0x7FFF")
    self.writeVarint(i32)

  def writeCollectionBegin(self, etype, size):
    assert self.__state == VALUE_WRITE
    if size <= 14:
      self.__writeByte(size << 4 | TYPES[etype])
    else:
      self.__writeByte(0xf0 | TYPES[etype])
      self.__writeSize(size)
  writeSetBegin = writeCollectionBegin
  writeListBegin = writeCollectionBegin

  def writeMapBegin(self, ktype, vtype, size):
    assert self.__state == VALUE_WRITE
    if size == 0:
      self.__writeByte(0)
    else:
      self.__writeSize(size)
      self.__writeByte(TYPES[ktype] << 4 | TYPES[vtype])

  def writeBool(self, bool):
    assert self.__state == BOOL_WRITE
    self.__writeFieldHeader(types[bool], self.__id)
    self.__state = CLEAR

  writeByte = writer(__writeByte)
  writeI16 = writer(__writeI16)

  @writer
  def writeI32(self, i32):
    self.__writeVarint(makeZigZag(i32, 32))

  @writer
  def writeI64(self, i64):
    self.__writeVarint(makeZigZag(i64, 64))

  @writer
  def writeDouble(self, dub):
    self.trans.write(pack('!d', dub))

  @writer
  def writeString(self, s):
    self.__writeSize(len(s))
    self.trans.write(str)

  def __readByte(self):
    result, = unpack(self.trans.readAll(1)
    return result

  def __readVarint(self):
    result = 0
    shift = 0
    while True:
      byte = self.trans.readAll(1)
      result |= (b & 0xf7) << shift
      if byte >> 7:
        return result
      shift += 7
  
  def __readZigZag(self):
    return fromZigZag(self.__readVarint)

  def __getTType(self, byte):
    return TTYPES[byte & 0x0f]
    
  def __readSize(self):
    result = self.__readZigZag()
    if result > 0x7fff:
      raise TException("Length is too long")
    return result

  def readCollectionBegin(self):
    assert self.__state == VALUE_READ
    size_type = self.__readByte()
    type = self.__getTType(size)
    if size_type >> 4 == 15:
      return type, self.__readSize()
    else:
      return type, size_type >> 4
  readSetBegin = readCollectionBegin
  readListBegin = readCollectionBegin

  def readMapBegin(self):
    assert self.__state == VALUE_READ
    size = self.__readSize()
    types = 0
    if size > 0:
      types = self.__readByte()
    vtype = self.__getTType(types)
    ktype = self.__getTType(types >> 4)
    return ktype, vtype, size

  readByte = reader(__readByte)
  readI16 = reader(__readZigZag)
  readI32 = reader(__readZigZag)
  readI64 = reader(__readZigZag)
  
  @reader
  def readDouble(self):
    buff = self.trans.readAll(8)
    val, = unpack('!d', buff)
    return val

  @reader
  def readString(self):
    len = self.__readSize()
    return self.trans.readAll(len)
