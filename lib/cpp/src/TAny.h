#ifndef _THRIFT_ANY_H_
#define _THRIFT_ANY_H_

#include <boost/any.hpp>
#include <string>
#include "Thrift.h"

namespace facebook { namespace thrift {
class TAny;
bool operator == (const facebook::thrift::TAny &lhs, const facebook::thrift::TAny &rhs);

class TAny {
 private:
  boost::any value_;
 public:
  TAny() : value_() {};
  TAny(int8_t val) : value_(val) {}
  TAny(int16_t val) : value_(val) {}
  TAny(int32_t val) : value_(val) {}
  TAny(int64_t val) : value_(val) {}
  TAny(const std::string &val) : value_(val) {}
  TAny(const ThriftBase *val) : value_(val) {}
  TAny(const TAny &val) : value_(val) {}
  virtual ~TAny() {/* fill it*/ };

  friend bool operator == (const TAny&, const TAny&);
  /*TAny & operator = (const TAny &rhs);*/
  bool operator != (const TAny &rhs) {
    return operator==(*this, rhs);
  }

  const std::type_info & type() const { return value_.type(); }
  uint32_t read(facebook::thrift::protocol::TProtocol *iprot);
  uint32_t write(facebook::thrift::protocol::TProtocol *oprot) const;
};

//TAny & operator = (const TAny &lhs, const TAny &rhs);
}}
#endif // _THRIFT_ANY_H_
