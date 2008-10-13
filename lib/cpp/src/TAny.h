#ifndef _THRIFT_ANY_H_
#define _THRIFT_ANY_H_

#include <boost/any.hpp>
#include <boost/pool/detail/singleton.hpp>
#include <map>
#include <string>
#include <Thrift.h>
#include <concurrency/Monitor.h>

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
  TAny & operator = (const TAny &rhs);
  bool operator != (const TAny &rhs) {
    return !(*this == rhs);
  }

  const std::type_info & type() const { return value_.type(); }
  uint32_t read(facebook::thrift::protocol::TProtocol *iprot);
  uint32_t write(facebook::thrift::protocol::TProtocol *oprot) const;
};

typedef ThriftBase* (*StructCreator)();
// i've check out boost::singleton_default: it's thread safe if thread runs inside main
// i belive that thread in thrift will work only in main().
class ThriftFactory_ {
 private:
  facebook::thrift::concurrency::Monitor monitor_;
  std::map<std::string, StructCreator> mapping_;
 public:
  ThriftFactory_() : monitor_(), mapping_() {}
  /**
   * Register class in factory, if you have class which derived from generated
   * thrift class you should re-register your class. It's not a problem.
   *
   * Returns true if previos class was registered under that md5.
   */
  bool registerStruct(const std::string &md5, StructCreator func);

  /**
   * If thrift structure is unknown it's okay, like versioning in the rest
   * of thrift, 'get' returns 0 in this case.
   */
  ThriftBase* get(const std::string &md5);
};

class ThriftFactory : public boost::details::pool::singleton_default<ThriftFactory_>
{
 public:
  static ThriftBase* get(const std::string &md5) {
    return instance().get(md5);
  }
  static bool registerStruct(const std::string &md5, StructCreator func) {
    return instance().registerStruct(md5, func);
  }
};
}}
#endif // _THRIFT_ANY_H_
