// Copyright (c) 2006- Facebook
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/

#include "TProtocol.h"


namespace facebook { namespace thrift { namespace protocol {

uint32_t TProtocol::skip(TType type) {
  switch (type) {
  case T_BOOL:
    {
      bool boolv;
      return readBool(boolv);
    }
  case T_BYTE:
    {
      int8_t bytev;
      return readByte(bytev);
    }
  case T_I16:
    {
      int16_t i16;
      return readI16(i16);
    }
  case T_I32:
    {
      int32_t i32;
      return readI32(i32);
    }
  case T_I64:
    {
      int64_t i64;
      return readI64(i64);
    }
  case T_DOUBLE:
    {
      double dub;
      return readDouble(dub);
    }
  case T_STRING:
    {
      std::string str;
      return readBinary(str);
    }
  case T_STRUCT:
    {
      uint32_t result = 0;
      std::string name;
      int16_t fid;
      TType ftype;
      result += readStructBegin(name);
      while (true) {
        result += readFieldBegin(name, ftype, fid);
        if (ftype == T_STOP) {
          break;
        }
        result += skip(ftype);
        result += readFieldEnd();
      }
      result += readStructEnd();
      return result;
    }
  case T_MAP:
    {
      uint32_t result = 0;
      TType keyType;
      TType valType;
      uint32_t i, size;
      result += readMapBegin(keyType, valType, size);
      for (i = 0; i < size; i++) {
        result += skip(keyType);
        result += skip(valType);
      }
      result += readMapEnd();
      return result;
    }
  case T_SET:
    {
      uint32_t result = 0;
      TType elemType;
      uint32_t i, size;
      result += readSetBegin(elemType, size);
      for (i = 0; i < size; i++) {
        result += skip(elemType);
      }
      result += readSetEnd();
      return result;
    }
  case T_LIST:
    {
      uint32_t result = 0;
      TType elemType;
      uint32_t i, size;
      result += readListBegin(elemType, size);
      for (i = 0; i < size; i++) {
        result += skip(elemType);
      }
      result += readListEnd();
      return result;
    }
  default:
    return 0;
  }
}

}}} // facebook::thrift::protocol
