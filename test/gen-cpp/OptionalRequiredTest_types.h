/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 */
#ifndef OptionalRequiredTest_TYPES_H
#define OptionalRequiredTest_TYPES_H

#include <Thrift.h>
#include <protocol/TProtocol.h>
#include <transport/TTransport.h>



namespace thrift { namespace test {

class OldSchool {
 public:

  static const char* ascii_fingerprint; // = "24510143CA4642328D8FDA8C850A3C36";
  static const uint8_t binary_fingerprint[16]; // = {0x24,0x51,0x01,0x43,0xCA,0x46,0x42,0x32,0x8D,0x8F,0xDA,0x8C,0x85,0x0A,0x3C,0x36};

  OldSchool() : im_int(0), im_str("") {
  }

  virtual ~OldSchool() throw() {}

  static ::apache::thrift::reflection::local::TypeSpec* local_reflection;

  int16_t im_int;
  std::string im_str;
  std::vector<std::map<int32_t, std::string> >  im_big;

  struct __isset {
    __isset() : im_int(false), im_str(false), im_big(false) {}
    bool im_int;
    bool im_str;
    bool im_big;
  } __isset;

  bool operator == (const OldSchool & rhs) const
  {
    if (!(im_int == rhs.im_int))
      return false;
    if (!(im_str == rhs.im_str))
      return false;
    if (!(im_big == rhs.im_big))
      return false;
    return true;
  }
  bool operator != (const OldSchool &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const OldSchool & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

// i16
extern ::apache::thrift::reflection::local::TypeSpec
trlo_typespec_OptionalRequiredTest_2655F23011F3FE4C7C3D757D37D5F9A5;

// string
extern ::apache::thrift::reflection::local::TypeSpec
trlo_typespec_OptionalRequiredTest_B45CFFE084DD3D20D928BEE85E7B0F21;

// i32
extern ::apache::thrift::reflection::local::TypeSpec
trlo_typespec_OptionalRequiredTest_F89EDB52B075B8BD95989BD3D0A04C0A;

// map<i32,string>
extern ::apache::thrift::reflection::local::TypeSpec
trlo_typespec_OptionalRequiredTest_6870A6B6F5D23566138BB497788EDCB9;

// list<map<i32,string>>
extern ::apache::thrift::reflection::local::TypeSpec
trlo_typespec_OptionalRequiredTest_E25A1560BCF4BF2587943D2F86734458;

// void
extern ::apache::thrift::reflection::local::TypeSpec
trlo_typespec_OptionalRequiredTest_CAB8111FD0B710A336C898E539090E34;

// {1:i16;2:string;3:list<map<i32,string>>;}
extern ::apache::thrift::reflection::local::TypeSpec
trlo_typespec_OptionalRequiredTest_24510143CA4642328D8FDA8C850A3C36;

class Simple {
 public:

  static const char* ascii_fingerprint; // = "BE81C8950133989CA99AAE12969A558C";
  static const uint8_t binary_fingerprint[16]; // = {0xBE,0x81,0xC8,0x95,0x01,0x33,0x98,0x9C,0xA9,0x9A,0xAE,0x12,0x96,0x9A,0x55,0x8C};

  Simple() : im_default(0), im_required(0), im_optional(0) {
  }

  virtual ~Simple() throw() {}

  static ::apache::thrift::reflection::local::TypeSpec* local_reflection;

  int16_t im_default;
  int16_t im_required;
  int16_t im_optional;

  struct __isset {
    __isset() : im_default(false), im_optional(false) {}
    bool im_default;
    bool im_optional;
  } __isset;

  bool operator == (const Simple & rhs) const
  {
    if (!(im_default == rhs.im_default))
      return false;
    if (!(im_required == rhs.im_required))
      return false;
    if (__isset.im_optional != rhs.__isset.im_optional)
      return false;
    else if (__isset.im_optional && !(im_optional == rhs.im_optional))
      return false;
    return true;
  }
  bool operator != (const Simple &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Simple & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

// {1:i16;2:i16;3:opt-i16;}
extern ::apache::thrift::reflection::local::TypeSpec
trlo_typespec_OptionalRequiredTest_BE81C8950133989CA99AAE12969A558C;

class Tricky1 {
 public:

  static const char* ascii_fingerprint; // = "565787C31CF2D774B532CB755189BF39";
  static const uint8_t binary_fingerprint[16]; // = {0x56,0x57,0x87,0xC3,0x1C,0xF2,0xD7,0x74,0xB5,0x32,0xCB,0x75,0x51,0x89,0xBF,0x39};

  Tricky1() : im_default(0) {
  }

  virtual ~Tricky1() throw() {}

  static ::apache::thrift::reflection::local::TypeSpec* local_reflection;

  int16_t im_default;

  struct __isset {
    __isset() : im_default(false) {}
    bool im_default;
  } __isset;

  bool operator == (const Tricky1 & rhs) const
  {
    if (!(im_default == rhs.im_default))
      return false;
    return true;
  }
  bool operator != (const Tricky1 &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Tricky1 & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

// {1:i16;}
extern ::apache::thrift::reflection::local::TypeSpec
trlo_typespec_OptionalRequiredTest_565787C31CF2D774B532CB755189BF39;

class Tricky2 {
 public:

  static const char* ascii_fingerprint; // = "5DAC9C51C7E1106BF936FC71860BE9D5";
  static const uint8_t binary_fingerprint[16]; // = {0x5D,0xAC,0x9C,0x51,0xC7,0xE1,0x10,0x6B,0xF9,0x36,0xFC,0x71,0x86,0x0B,0xE9,0xD5};

  Tricky2() : im_optional(0) {
  }

  virtual ~Tricky2() throw() {}

  static ::apache::thrift::reflection::local::TypeSpec* local_reflection;

  int16_t im_optional;

  struct __isset {
    __isset() : im_optional(false) {}
    bool im_optional;
  } __isset;

  bool operator == (const Tricky2 & rhs) const
  {
    if (__isset.im_optional != rhs.__isset.im_optional)
      return false;
    else if (__isset.im_optional && !(im_optional == rhs.im_optional))
      return false;
    return true;
  }
  bool operator != (const Tricky2 &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Tricky2 & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

// {1:opt-i16;}
extern ::apache::thrift::reflection::local::TypeSpec
trlo_typespec_OptionalRequiredTest_5DAC9C51C7E1106BF936FC71860BE9D5;

class Tricky3 {
 public:

  static const char* ascii_fingerprint; // = "565787C31CF2D774B532CB755189BF39";
  static const uint8_t binary_fingerprint[16]; // = {0x56,0x57,0x87,0xC3,0x1C,0xF2,0xD7,0x74,0xB5,0x32,0xCB,0x75,0x51,0x89,0xBF,0x39};

  Tricky3() : im_required(0) {
  }

  virtual ~Tricky3() throw() {}

  static ::apache::thrift::reflection::local::TypeSpec* local_reflection;

  int16_t im_required;

  bool operator == (const Tricky3 & rhs) const
  {
    if (!(im_required == rhs.im_required))
      return false;
    return true;
  }
  bool operator != (const Tricky3 &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Tricky3 & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

class Complex {
 public:

  static const char* ascii_fingerprint; // = "F2D7976304A5302593F62D9B33898375";
  static const uint8_t binary_fingerprint[16]; // = {0xF2,0xD7,0x97,0x63,0x04,0xA5,0x30,0x25,0x93,0xF6,0x2D,0x9B,0x33,0x89,0x83,0x75};

  Complex() : cp_default(0), cp_required(0), cp_optional(0) {
  }

  virtual ~Complex() throw() {}

  static ::apache::thrift::reflection::local::TypeSpec* local_reflection;

  int16_t cp_default;
  int16_t cp_required;
  int16_t cp_optional;
  std::map<int16_t, Simple>  the_map;
  Simple req_simp;
  Simple opt_simp;

  struct __isset {
    __isset() : cp_default(false), cp_optional(false), the_map(false), opt_simp(false) {}
    bool cp_default;
    bool cp_optional;
    bool the_map;
    bool opt_simp;
  } __isset;

  bool operator == (const Complex & rhs) const
  {
    if (!(cp_default == rhs.cp_default))
      return false;
    if (!(cp_required == rhs.cp_required))
      return false;
    if (__isset.cp_optional != rhs.__isset.cp_optional)
      return false;
    else if (__isset.cp_optional && !(cp_optional == rhs.cp_optional))
      return false;
    if (!(the_map == rhs.the_map))
      return false;
    if (!(req_simp == rhs.req_simp))
      return false;
    if (__isset.opt_simp != rhs.__isset.opt_simp)
      return false;
    else if (__isset.opt_simp && !(opt_simp == rhs.opt_simp))
      return false;
    return true;
  }
  bool operator != (const Complex &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Complex & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

// map<i16,{1:i16;2:i16;3:opt-i16;}>
extern ::apache::thrift::reflection::local::TypeSpec
trlo_typespec_OptionalRequiredTest_2AF2A0DB79249912661A3E072EA4A70C;

// {1:i16;2:i16;3:opt-i16;4:map<i16,{1:i16;2:i16;3:opt-i16;}>;5:{1:i16;2:i16;3:opt-i16;};6:opt-{1:i16;2:i16;3:opt-i16;};}
extern ::apache::thrift::reflection::local::TypeSpec
trlo_typespec_OptionalRequiredTest_F2D7976304A5302593F62D9B33898375;

class ManyOpt {
 public:

  static const char* ascii_fingerprint; // = "8CA8A5797EF1B7D3A6A7F5E0E4B7B5AF";
  static const uint8_t binary_fingerprint[16]; // = {0x8C,0xA8,0xA5,0x79,0x7E,0xF1,0xB7,0xD3,0xA6,0xA7,0xF5,0xE0,0xE4,0xB7,0xB5,0xAF};

  ManyOpt() : opt1(0), opt2(0), opt3(0), def4(0), opt5(0), opt6(0) {
  }

  virtual ~ManyOpt() throw() {}

  static ::apache::thrift::reflection::local::TypeSpec* local_reflection;

  int32_t opt1;
  int32_t opt2;
  int32_t opt3;
  int32_t def4;
  int32_t opt5;
  int32_t opt6;

  struct __isset {
    __isset() : opt1(false), opt2(false), opt3(false), def4(false), opt5(false), opt6(false) {}
    bool opt1;
    bool opt2;
    bool opt3;
    bool def4;
    bool opt5;
    bool opt6;
  } __isset;

  bool operator == (const ManyOpt & rhs) const
  {
    if (__isset.opt1 != rhs.__isset.opt1)
      return false;
    else if (__isset.opt1 && !(opt1 == rhs.opt1))
      return false;
    if (__isset.opt2 != rhs.__isset.opt2)
      return false;
    else if (__isset.opt2 && !(opt2 == rhs.opt2))
      return false;
    if (__isset.opt3 != rhs.__isset.opt3)
      return false;
    else if (__isset.opt3 && !(opt3 == rhs.opt3))
      return false;
    if (!(def4 == rhs.def4))
      return false;
    if (__isset.opt5 != rhs.__isset.opt5)
      return false;
    else if (__isset.opt5 && !(opt5 == rhs.opt5))
      return false;
    if (__isset.opt6 != rhs.__isset.opt6)
      return false;
    else if (__isset.opt6 && !(opt6 == rhs.opt6))
      return false;
    return true;
  }
  bool operator != (const ManyOpt &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ManyOpt & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

// {1:opt-i32;2:opt-i32;3:opt-i32;4:i32;5:opt-i32;6:opt-i32;}
extern ::apache::thrift::reflection::local::TypeSpec
trlo_typespec_OptionalRequiredTest_8CA8A5797EF1B7D3A6A7F5E0E4B7B5AF;

class JavaTestHelper {
 public:

  static const char* ascii_fingerprint; // = "2FA9BE24A2F66387CD678E6D2F739A02";
  static const uint8_t binary_fingerprint[16]; // = {0x2F,0xA9,0xBE,0x24,0xA2,0xF6,0x63,0x87,0xCD,0x67,0x8E,0x6D,0x2F,0x73,0x9A,0x02};

  JavaTestHelper() : req_int(0), opt_int(0), req_obj(""), opt_obj(""), req_bin(""), opt_bin("") {
  }

  virtual ~JavaTestHelper() throw() {}

  static ::apache::thrift::reflection::local::TypeSpec* local_reflection;

  int32_t req_int;
  int32_t opt_int;
  std::string req_obj;
  std::string opt_obj;
  std::string req_bin;
  std::string opt_bin;

  struct __isset {
    __isset() : opt_int(false), opt_obj(false), opt_bin(false) {}
    bool opt_int;
    bool opt_obj;
    bool opt_bin;
  } __isset;

  bool operator == (const JavaTestHelper & rhs) const
  {
    if (!(req_int == rhs.req_int))
      return false;
    if (__isset.opt_int != rhs.__isset.opt_int)
      return false;
    else if (__isset.opt_int && !(opt_int == rhs.opt_int))
      return false;
    if (!(req_obj == rhs.req_obj))
      return false;
    if (__isset.opt_obj != rhs.__isset.opt_obj)
      return false;
    else if (__isset.opt_obj && !(opt_obj == rhs.opt_obj))
      return false;
    if (!(req_bin == rhs.req_bin))
      return false;
    if (__isset.opt_bin != rhs.__isset.opt_bin)
      return false;
    else if (__isset.opt_bin && !(opt_bin == rhs.opt_bin))
      return false;
    return true;
  }
  bool operator != (const JavaTestHelper &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const JavaTestHelper & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

// {1:i32;2:opt-i32;3:string;4:opt-string;5:string;6:opt-string;}
extern ::apache::thrift::reflection::local::TypeSpec
trlo_typespec_OptionalRequiredTest_2FA9BE24A2F66387CD678E6D2F739A02;

}} // namespace

#endif
