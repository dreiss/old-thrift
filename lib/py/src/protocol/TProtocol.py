#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements. See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership. The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License. You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied. See the License for the
# specific language governing permissions and limitations
# under the License.
#

from thrift.Thrift import *

__all__ = ['TProtocolException', 'TProtocolBase', 'TProtocolFactory']

def write_helper(oprot, ftype, value):
  # copy-paste from TProtocol.skip
  if ftype == TType.STOP:
    raise "XXX"
  elif ftype == TType.BOOL:
    oprot.writeBool(value)
  elif ftype == TType.BYTE:
    oprot.writeByte(value)
  elif ftype == TType.I16:
    oprot.writeI16(value)
  elif ftype == TType.I32:
    oprot.writeI32(value)
  elif ftype == TType.I64:
    oprot.writeI64(value)
  elif ftype == TType.DOUBLE:
    oprot.writeDouble(value)
  elif ftype == TType.STRING:
    oprot.writeString(value)
  else:
    raise "XXX: hey, it's %d type" % ftype

def reader_helper(iprot, ftype):
  # copy-paste from TProtocol.skip
  if ftype == TType.STOP:
    raise "XXX"
  elif ftype == TType.BOOL:
    return iprot.readBool()
  elif ftype == TType.BYTE:
    return iprot.readByte()
  elif ftype == TType.I16:
    return iprot.readI16()
  elif ftype == TType.I32:
    return iprot.readI32()
  elif ftype == TType.I64:
    return iprot.readI64()
  elif ftype == TType.DOUBLE:
    return iprot.readDouble()
  elif ftype == TType.STRING:
    return iprot.readString()
  elif ftype == TType.MAP:
    # XXX TODO: support complex types
    ktype, vtype, size = iprot.readMapBegin()
    result = {}
    for i in range(size):
      key = iprot.skip(ktype)
      value = iprot.skip(vtype)
      result[key] = value
    iprot.readMapEnd()
    return result
  elif ftype == TType.SET:
    etype, size = iprot.readSetBegin()
    result = set()
    for i in xrange(size):
      result.update([reader_helper(iprot, etype)])
    iprot.readSetEnd()
    return result
  elif ftype == TType.LIST:
    etype, size = iprot.readListBegin()
    result = []
    for i in xrange(size):
      result.append(reader_helper(iprot, etype))
    iprot.readListEnd()
    return result
  raise "XXX"

def write_adv_helper(oprot, ftype, type_args, value):
  if ftype == TType.MAP:
    # XXX TODO handle complex type
    ktype, _, vtype, _ = type_args
    oprot.writeMapBegin(ktype, vtype, len(value))
    for k, v in value.items():
      write_helper(oprot, ktype, k)
      write_helper(oprot, vtype, v)
    oprot.writeMapEnd()
  elif ftype in (TType.LIST, TType.SET):
    if ftype == TType.LIST:
      begin = oprot.writeListBegin
      end = oprot.writeListEnd
    else:
      begin = oprot.writeSetBegin
      end = oprot.writeSetEnd
    xtype, _ = type_args
    begin(xtype, len(value))
    for k in value:
      write_helper(oprot, xtype, k)
    end()
  else:
    raise "XXX"

class TProtocolException(TException):

  """Custom Protocol Exception class"""

  UNKNOWN = 0
  INVALID_DATA = 1
  NEGATIVE_SIZE = 2
  SIZE_LIMIT = 3
  BAD_VERSION = 4

  def __init__(self, type=UNKNOWN, message=None):
    TException.__init__(self, message)
    self.type = type

class TProtocolBase:

  """Base class for Thrift protocol driver."""

  def __init__(self, trans):
    self.trans = trans

  def writeMessageBegin(self, name, type, seqid):
    pass

  def writeMessageEnd(self):
    pass

  def writeStructBegin(self, name):
    pass

  def writeStructEnd(self):
    pass

  def writeFieldBegin(self, name, type, id):
    pass

  def writeFieldEnd(self):
    pass

  def writeFieldStop(self):
    pass

  def writeMapBegin(self, ktype, vtype, size):
    pass

  def writeMapEnd(self):
    pass

  def writeListBegin(self, etype, size):
    pass

  def writeListEnd(self):
    pass

  def writeSetBegin(self, etype, size):
    pass

  def writeSetEnd(self):
    pass

  def writeBool(self, bool):
    pass

  def writeByte(self, byte):
    pass

  def writeI16(self, i16):
    pass

  def writeI32(self, i32):
    pass

  def writeI64(self, i64):
    pass

  def writeDouble(self, dub):
    pass

  def writeString(self, str):
    pass

  def readMessageBegin(self):
    pass

  def readMessageEnd(self):
    pass

  def readStructBegin(self):
    pass

  def readStructEnd(self):
    pass

  def readFieldBegin(self):
    pass

  def readFieldEnd(self):
    pass

  def readMapBegin(self):
    pass

  def readMapEnd(self):
    pass

  def readListBegin(self):
    pass

  def readListEnd(self):
    pass

  def readSetBegin(self):
    pass

  def readSetEnd(self):
    pass

  def readBool(self):
    pass

  def readByte(self):
    pass

  def readI16(self):
    pass

  def readI32(self):
    pass

  def readI64(self):
    pass

  def readDouble(self):
    pass

  def readString(self):
    pass

  def skip(self, type):
    if type == TType.STOP:
      return
    elif type == TType.BOOL:
      self.readBool()
    elif type == TType.BYTE:
      self.readByte()
    elif type == TType.I16:
      self.readI16()
    elif type == TType.I32:
      self.readI32()
    elif type == TType.I64:
      self.readI64()
    elif type == TType.DOUBLE:
      self.readDouble()
    elif type == TType.STRING:
      self.readString()
    elif type == TType.STRUCT:
      name = self.readStructBegin()
      while True:
        (name, type, id) = self.readFieldBegin()
        if type == TType.STOP:
          break
        self.skip(type)
        self.readFieldEnd()
      self.readStructEnd()
    elif type == TType.MAP:
      (ktype, vtype, size) = self.readMapBegin()
      for i in range(size):
        self.skip(ktype)
        self.skip(vtype)
      self.readMapEnd()
    elif type == TType.SET:
      (etype, size) = self.readSetBegin()
      for i in range(size):
        self.skip(etype)
      self.readSetEnd()
    elif type == TType.LIST:
      (etype, size) = self.readListBegin()
      for i in range(size):
        self.skip(etype)
      self.readListEnd()

  def read(self, struct):
    self.readStructBegin()
    spec = struct.thrift_spec
    offset = struct.thrift_offset
    while True:
      fname, ftype, fid = self.readFieldBegin()
      if ftype == TType.STOP:
        break
      else:
        sid, stype, sname, type_args, default = spec[fid - offset]
        if stype == ftype:
          if ftype == TType.STRUCT:
            class_, _ = type_args
            result = class_()
            result.read(self)
            setattr(struct, sname, result)
          else:
            setattr(struct, sname, reader_helper(self.prot, stype))
        else:
          self.skip(ftype)
    self.readFieldEnd()
    self.readStructEnd()

  def write(self, struct):
    self.writeStructBegin(self.__class__.__name__)
    for spec in struct.thrift_spec:
      if spec is None:
        continue
      fid, ftype, fname, type_args, default = spec
      value = getattr(struct, fname, default)
      if value is not None: # it's bad idea to skip [] as None
        self.writeFieldBegin(fname, ftype, fid)
        if ftype in (TType.SET, TType.MAP, TType.LIST):
          write_adv_helper(self.trans, ftype, type_args, value)
        elif ftype == TType.STRUCT:
          value.write(self)
        else:
          write_helper(seld.trans, ftype, value)
        self.writeFieldEnd()
    self.writeFieldStop()
    self.writeStructEnd()

class TProtocolFactory:
  def getProtocol(self, trans):
    pass
