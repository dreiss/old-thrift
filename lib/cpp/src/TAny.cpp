#include "TAny.h"
#include <protocol/TProtocol.h>

using namespace facebook::thrift;
using namespace facebook::thrift::protocol;

uint8_t guess_type(const TAny &val);

namespace facebook { namespace thrift {
bool operator == (const TAny &lhs, const TAny &rhs)
{
  if (lhs.type() != rhs.type())
    return false;
  if (lhs.type() == typeid(int8_t))
    return boost::any_cast<int8_t>(lhs.value_) == boost::any_cast<int8_t>(rhs.value_);
  if (lhs.type() == typeid(bool))
    return boost::any_cast<bool>(lhs.value_) == boost::any_cast<bool>(rhs.value_);
  if (lhs.type() == typeid(int16_t))
    return boost::any_cast<int16_t>(lhs.value_) == boost::any_cast<int16_t>(rhs.value_);
  if (lhs.type() == typeid(int32_t))
    return boost::any_cast<int32_t>(lhs.value_) == boost::any_cast<int32_t>(rhs.value_);
  if (lhs.type() == typeid(int64_t))
    return boost::any_cast<int64_t>(lhs.value_) == boost::any_cast<int64_t>(rhs.value_);
  if (lhs.type() == typeid(std::string))
    return boost::any_cast<std::string>(lhs.value_) == boost::any_cast<std::string>(rhs.value_);
  if (lhs.type() == typeid(ThriftBase*)) {
    ThriftBase *left = boost::any_cast<ThriftBase*>(lhs.value_);
    ThriftBase *right = boost::any_cast<ThriftBase*>(rhs.value_);
    return *left == *right;
  }
  throw "can't guess type";
}

using facebook::thrift::concurrency::Synchronized;

bool ThriftFactory_::registerStruct(const std::string &md5, StructCreator func) {
  Synchronized s(monitor_);
  mapping_[md5] = func;
  return true;
}

ThriftBase* ThriftFactory_::get(const std::string &md5) {
  Synchronized s(monitor_);
  std::map<std::string, StructCreator>::iterator i = mapping_.find(md5);
  if (i == mapping_.end())
    return 0;
  return i->second();
}
}}

/*bool TAny::operator = (const TAny &rhs) {
}*/
/*TAny & operator = (const TAny &lhs, const TAny &rhs)
{
  lhs.value_ = rhs.value_;
}*/

uint32_t TAny::write(TProtocol *oprot) const {
  int8_t type = guess_type(*this);
  uint32_t result = oprot->writeByte(type);
  ThriftBase *t_struct;
  printf("guessed type is %d\n", type);
  switch (type) {
    case T_BYTE:
      return result + oprot->writeByte(boost::any_cast<int8_t>(value_));
    case T_BOOL:
      return result + oprot->writeBool(boost::any_cast<bool>(value_));
    case T_I16:
      return result + oprot->writeI16(boost::any_cast<int16_t>(value_));
    case T_I32:
      return result + oprot->writeI32(boost::any_cast<int32_t>(value_));
    case T_I64:
      return result + oprot->writeI64(boost::any_cast<int64_t>(value_));
    case T_DOUBLE:
      return result + oprot->writeDouble(boost::any_cast<double>(value_));
    case T_STRING:
      return result + oprot->writeString(boost::any_cast<std::string>(value_));
    case T_STRUCT:
      t_struct = boost::any_cast<ThriftBase*>(value_);
      result += t_struct->writeFingerPrint(oprot);
      return result + t_struct->write(oprot);
    default:
        // TODO(shigin): fill it
        throw "not implemented (yet)";
  }
}

uint32_t TAny::read(TProtocol *iprot) {
  int8_t type;
  uint32_t result = iprot->readByte(type);
  char finger[32];
  ThriftBase *tmp;
  std::string str = std::string();
  switch (type) {
    case T_BYTE:
      int8_t bt;
      result += iprot->readByte(bt);
      value_ = bt;
      break;
    case T_BOOL:
      bool bl;
      result += iprot->readBool(bl);
      value_ = bl;
      break;
    case T_I16:
      int16_t i1;
      result += iprot->readI16(i1);
      value_ = i1;
      break;
    case T_I32:
      int32_t i2;
      result += iprot->readI32(i2);
      value_ = i2;
      break;
    case T_I64:
      int64_t i4;
      result += iprot->readI64(i4);
      value_ = i4;
      break;
    case T_DOUBLE:
      double db;
      result += iprot->readDouble(db);
      value_ = db;
      break;
    case T_STRING:
      result += iprot->readString(str);
      value_ = str;
      break;
    case T_STRUCT:
      int8_t x;
      // TODO(shigin): it's aweful, fix it
      for (int i=0; i<16; ++i)
      {
        result += iprot->readByte(x);
        uint8_t hi = (uint8_t)x / 32; // is it safe???
        uint8_t lo = (uint8_t)x % 32;
        finger[i*2] = (hi > 9) ? hi + 'A' - 10 : hi + '0';
        finger[i*2] = (lo > 9) ? lo + 'A' - 10 : lo + '0';
      }
      tmp = ThriftFactory::get(finger);
      if (tmp != 0)
      {
        result += tmp->read(iprot);
        value_ = tmp;
      } else {
        result += iprot->skip(T_STRUCT);
      }
      break;
    default:
      // TODO(shigin): fill it
      throw "not implemented (yet)";
  }
  return result;
}

uint8_t guess_type(const TAny &val)
{
  if (val.type() == typeid(int8_t))
    return T_BYTE;
  if (val.type() == typeid(bool))
    return T_BOOL;
  if (val.type() == typeid(int16_t))
    return T_I16;
  if (val.type() == typeid(int32_t))
    return T_I32;
  if (val.type() == typeid(int64_t))
    return T_I64;
  if (val.type() == typeid(std::string))
    return T_STRING;
  if (val.type() == typeid(ThriftBase*))
    return T_STRUCT;
  fprintf(stderr, "sorry, i can't handle %s type\n", val.type().name());
  throw "can't guess type";
}
