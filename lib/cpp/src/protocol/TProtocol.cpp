#include <protocol/TProtocol.h>

namespace facebook { namespace thrift { namespace protocol {

uint8_t guess_type(const boost::any& val)
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
  fprintf(stderr, "sorry, i can't handle %s type\n", val.type().name());
  throw "can't guess type";
}
}}}
