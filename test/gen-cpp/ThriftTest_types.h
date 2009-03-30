/**
 * Autogenerated by Thrift
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 */
#ifndef ThriftTest_TYPES_H
#define ThriftTest_TYPES_H

#include <Thrift.h>
#include <protocol/TProtocol.h>
#include <transport/TTransport.h>



namespace thrift { namespace test {

enum Numberz {
  ONE = 1,
  TWO,
  THREE,
  FIVE = 5,
  SIX,
  EIGHT = 8
};

// enum
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_DA45EC4BE6574774008DF9BE683A4778;

typedef int64_t UserId;

class Bonk {
 public:

  static const char* ascii_fingerprint; // = "EEBC915CE44901401D881E6091423036";
  static const uint8_t binary_fingerprint[16]; // = {0xEE,0xBC,0x91,0x5C,0xE4,0x49,0x01,0x40,0x1D,0x88,0x1E,0x60,0x91,0x42,0x30,0x36};

  Bonk() : message(""), type(0) {
  }

  virtual ~Bonk() throw() {}

  static apache::thrift::reflection::local::TypeSpec* local_reflection;

  std::string message;
  int32_t type;

  struct __isset {
    __isset() : message(false), type(false) {}
    bool message;
    bool type;
  } __isset;

  bool operator == (const Bonk & rhs) const
  {
    if (!(message == rhs.message))
      return false;
    if (!(type == rhs.type))
      return false;
    return true;
  }
  bool operator != (const Bonk &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Bonk & ) const;

  uint32_t read(apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(apache::thrift::protocol::TProtocol* oprot) const;

};

// string
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_B45CFFE084DD3D20D928BEE85E7B0F21;

// i32
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_F89EDB52B075B8BD95989BD3D0A04C0A;

// void
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_CAB8111FD0B710A336C898E539090E34;

// {1:string;2:i32;}
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_EEBC915CE44901401D881E6091423036;

class Bools {
 public:

  static const char* ascii_fingerprint; // = "403F0C4586060E367DA428DD09C59C9C";
  static const uint8_t binary_fingerprint[16]; // = {0x40,0x3F,0x0C,0x45,0x86,0x06,0x0E,0x36,0x7D,0xA4,0x28,0xDD,0x09,0xC5,0x9C,0x9C};

  Bools() : im_true(0), im_false(0) {
  }

  virtual ~Bools() throw() {}

  static apache::thrift::reflection::local::TypeSpec* local_reflection;

  bool im_true;
  bool im_false;

  struct __isset {
    __isset() : im_true(false), im_false(false) {}
    bool im_true;
    bool im_false;
  } __isset;

  bool operator == (const Bools & rhs) const
  {
    if (!(im_true == rhs.im_true))
      return false;
    if (!(im_false == rhs.im_false))
      return false;
    return true;
  }
  bool operator != (const Bools &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Bools & ) const;

  uint32_t read(apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(apache::thrift::protocol::TProtocol* oprot) const;

};

// bool
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_C506FF134BABDD6E68AB3E6350E95305;

// {1:bool;2:bool;}
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_403F0C4586060E367DA428DD09C59C9C;

class Xtruct {
 public:

  static const char* ascii_fingerprint; // = "01AAAA2258347790947AE6F75EE27B67";
  static const uint8_t binary_fingerprint[16]; // = {0x01,0xAA,0xAA,0x22,0x58,0x34,0x77,0x90,0x94,0x7A,0xE6,0xF7,0x5E,0xE2,0x7B,0x67};

  Xtruct() : string_thing(""), byte_thing(0), i32_thing(0), i64_thing(0) {
  }

  virtual ~Xtruct() throw() {}

  static apache::thrift::reflection::local::TypeSpec* local_reflection;

  std::string string_thing;
  int8_t byte_thing;
  int32_t i32_thing;
  int64_t i64_thing;

  struct __isset {
    __isset() : string_thing(false), byte_thing(false), i32_thing(false), i64_thing(false) {}
    bool string_thing;
    bool byte_thing;
    bool i32_thing;
    bool i64_thing;
  } __isset;

  bool operator == (const Xtruct & rhs) const
  {
    if (!(string_thing == rhs.string_thing))
      return false;
    if (!(byte_thing == rhs.byte_thing))
      return false;
    if (!(i32_thing == rhs.i32_thing))
      return false;
    if (!(i64_thing == rhs.i64_thing))
      return false;
    return true;
  }
  bool operator != (const Xtruct &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Xtruct & ) const;

  uint32_t read(apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(apache::thrift::protocol::TProtocol* oprot) const;

};

// byte
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_40EA57D3EE3C07BF1C102B466E1C3091;

// i64
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_DD1D26230D15C93E8FBDBA07B0299A44;

// {1:string;4:byte;9:i32;11:i64;}
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_01AAAA2258347790947AE6F75EE27B67;

class Xtruct2 {
 public:

  static const char* ascii_fingerprint; // = "0FA9E7DB20337B1935DECB146BE27561";
  static const uint8_t binary_fingerprint[16]; // = {0x0F,0xA9,0xE7,0xDB,0x20,0x33,0x7B,0x19,0x35,0xDE,0xCB,0x14,0x6B,0xE2,0x75,0x61};

  Xtruct2() : byte_thing(0), i32_thing(0) {
  }

  virtual ~Xtruct2() throw() {}

  static apache::thrift::reflection::local::TypeSpec* local_reflection;

  int8_t byte_thing;
  Xtruct struct_thing;
  int32_t i32_thing;

  struct __isset {
    __isset() : byte_thing(false), struct_thing(false), i32_thing(false) {}
    bool byte_thing;
    bool struct_thing;
    bool i32_thing;
  } __isset;

  bool operator == (const Xtruct2 & rhs) const
  {
    if (!(byte_thing == rhs.byte_thing))
      return false;
    if (!(struct_thing == rhs.struct_thing))
      return false;
    if (!(i32_thing == rhs.i32_thing))
      return false;
    return true;
  }
  bool operator != (const Xtruct2 &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Xtruct2 & ) const;

  uint32_t read(apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(apache::thrift::protocol::TProtocol* oprot) const;

};

// {1:byte;2:{1:string;4:byte;9:i32;11:i64;};3:i32;}
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_0FA9E7DB20337B1935DECB146BE27561;

class Insanity {
 public:

  static const char* ascii_fingerprint; // = "9E7D0680FA87F83E5A0CEF688569EFE3";
  static const uint8_t binary_fingerprint[16]; // = {0x9E,0x7D,0x06,0x80,0xFA,0x87,0xF8,0x3E,0x5A,0x0C,0xEF,0x68,0x85,0x69,0xEF,0xE3};

  Insanity() {
  }

  virtual ~Insanity() throw() {}

  static apache::thrift::reflection::local::TypeSpec* local_reflection;

  std::map<Numberz, UserId>  userMap;
  std::vector<Xtruct>  xtructs;

  struct __isset {
    __isset() : userMap(false), xtructs(false) {}
    bool userMap;
    bool xtructs;
  } __isset;

  bool operator == (const Insanity & rhs) const
  {
    if (!(userMap == rhs.userMap))
      return false;
    if (!(xtructs == rhs.xtructs))
      return false;
    return true;
  }
  bool operator != (const Insanity &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Insanity & ) const;

  uint32_t read(apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(apache::thrift::protocol::TProtocol* oprot) const;

};

// map<enum,i64>
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_C337A52D3836CAD9B459DC3BAE6A56DF;

// list<{1:string;4:byte;9:i32;11:i64;}>
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_30BCF193B507A8502F198CA671E26E64;

// {1:map<enum,i64>;2:list<{1:string;4:byte;9:i32;11:i64;}>;}
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_9E7D0680FA87F83E5A0CEF688569EFE3;

class CrazyNesting {
 public:

  static const char* ascii_fingerprint; // = "D2145FBF82608CCD4A1D0E5B6F34C02B";
  static const uint8_t binary_fingerprint[16]; // = {0xD2,0x14,0x5F,0xBF,0x82,0x60,0x8C,0xCD,0x4A,0x1D,0x0E,0x5B,0x6F,0x34,0xC0,0x2B};

  CrazyNesting() : string_field("") {
  }

  virtual ~CrazyNesting() throw() {}

  static apache::thrift::reflection::local::TypeSpec* local_reflection;

  std::string string_field;
  std::set<Insanity>  set_field;
  std::vector<std::map<std::set<int32_t> , std::map<int32_t, std::set<std::vector<std::map<Insanity, std::string> > > > > >  list_field;

  struct __isset {
    __isset() : string_field(false), set_field(false) {}
    bool string_field;
    bool set_field;
  } __isset;

  bool operator == (const CrazyNesting & rhs) const
  {
    if (!(string_field == rhs.string_field))
      return false;
    if (__isset.set_field != rhs.__isset.set_field)
      return false;
    else if (__isset.set_field && !(set_field == rhs.set_field))
      return false;
    if (!(list_field == rhs.list_field))
      return false;
    return true;
  }
  bool operator != (const CrazyNesting &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const CrazyNesting & ) const;

  uint32_t read(apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(apache::thrift::protocol::TProtocol* oprot) const;

};

// set<{1:map<enum,i64>;2:list<{1:string;4:byte;9:i32;11:i64;}>;}>
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_8A576F1408FB89A71D210397A83DDF47;

// set<i32>
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_698CD5CC80596EF059310FAB605A4491;

// map<{1:map<enum,i64>;2:list<{1:string;4:byte;9:i32;11:i64;}>;},string>
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_2A51D1822A16C77139F82F472BB1DB6C;

// list<map<{1:map<enum,i64>;2:list<{1:string;4:byte;9:i32;11:i64;}>;},string>>
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_E21ABAD2A20D2AC001D4A51B97D38E44;

// set<list<map<{1:map<enum,i64>;2:list<{1:string;4:byte;9:i32;11:i64;}>;},string>>>
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_A63F2609311B130CE9F9D040E65E233A;

// map<i32,set<list<map<{1:map<enum,i64>;2:list<{1:string;4:byte;9:i32;11:i64;}>;},string>>>>
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_BA4B91DC601BEC3DD80F041C071589B8;

// map<set<i32>,map<i32,set<list<map<{1:map<enum,i64>;2:list<{1:string;4:byte;9:i32;11:i64;}>;},string>>>>>
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_DD0011B9746E1C0358D4DCCB76D97926;

// list<map<set<i32>,map<i32,set<list<map<{1:map<enum,i64>;2:list<{1:string;4:byte;9:i32;11:i64;}>;},string>>>>>>
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_DF6F4B69A07B3B1A159588653F7865A8;

// {1:string;2:opt-set<{1:map<enum,i64>;2:list<{1:string;4:byte;9:i32;11:i64;}>;}>;3:list<map<set<i32>,map<i32,set<list<map<{1:map<enum,i64>;2:list<{1:string;4:byte;9:i32;11:i64;}>;},string>>>>>>;}
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_D2145FBF82608CCD4A1D0E5B6F34C02B;

class Xception : public apache::thrift::TException {
 public:

  static const char* ascii_fingerprint; // = "3F5FC93B338687BC7235B1AB103F47B3";
  static const uint8_t binary_fingerprint[16]; // = {0x3F,0x5F,0xC9,0x3B,0x33,0x86,0x87,0xBC,0x72,0x35,0xB1,0xAB,0x10,0x3F,0x47,0xB3};

  Xception() : errorCode(0), message("") {
  }

  virtual ~Xception() throw() {}

  static apache::thrift::reflection::local::TypeSpec* local_reflection;

  int32_t errorCode;
  std::string message;

  struct __isset {
    __isset() : errorCode(false), message(false) {}
    bool errorCode;
    bool message;
  } __isset;

  bool operator == (const Xception & rhs) const
  {
    if (!(errorCode == rhs.errorCode))
      return false;
    if (!(message == rhs.message))
      return false;
    return true;
  }
  bool operator != (const Xception &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Xception & ) const;

  uint32_t read(apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(apache::thrift::protocol::TProtocol* oprot) const;

};

// {1:i32;2:string;}
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_3F5FC93B338687BC7235B1AB103F47B3;

class Xception2 : public apache::thrift::TException {
 public:

  static const char* ascii_fingerprint; // = "7B1B1DBEB6FD136E8478F45D0399F3C4";
  static const uint8_t binary_fingerprint[16]; // = {0x7B,0x1B,0x1D,0xBE,0xB6,0xFD,0x13,0x6E,0x84,0x78,0xF4,0x5D,0x03,0x99,0xF3,0xC4};

  Xception2() : errorCode(0) {
  }

  virtual ~Xception2() throw() {}

  static apache::thrift::reflection::local::TypeSpec* local_reflection;

  int32_t errorCode;
  Xtruct struct_thing;

  struct __isset {
    __isset() : errorCode(false), struct_thing(false) {}
    bool errorCode;
    bool struct_thing;
  } __isset;

  bool operator == (const Xception2 & rhs) const
  {
    if (!(errorCode == rhs.errorCode))
      return false;
    if (!(struct_thing == rhs.struct_thing))
      return false;
    return true;
  }
  bool operator != (const Xception2 &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const Xception2 & ) const;

  uint32_t read(apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(apache::thrift::protocol::TProtocol* oprot) const;

};

// {1:i32;2:{1:string;4:byte;9:i32;11:i64;};}
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_7B1B1DBEB6FD136E8478F45D0399F3C4;

class EmptyStruct {
 public:

  static const char* ascii_fingerprint; // = "99914B932BD37A50B983C5E7C90AE93B";
  static const uint8_t binary_fingerprint[16]; // = {0x99,0x91,0x4B,0x93,0x2B,0xD3,0x7A,0x50,0xB9,0x83,0xC5,0xE7,0xC9,0x0A,0xE9,0x3B};

  EmptyStruct() {
  }

  virtual ~EmptyStruct() throw() {}

  static apache::thrift::reflection::local::TypeSpec* local_reflection;


  bool operator == (const EmptyStruct & /* rhs */) const
  {
    return true;
  }
  bool operator != (const EmptyStruct &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const EmptyStruct & ) const;

  uint32_t read(apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(apache::thrift::protocol::TProtocol* oprot) const;

};

// {}
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_99914B932BD37A50B983C5E7C90AE93B;

class OneField {
 public:

  static const char* ascii_fingerprint; // = "2C32A565725135A023CB591D1A993CFD";
  static const uint8_t binary_fingerprint[16]; // = {0x2C,0x32,0xA5,0x65,0x72,0x51,0x35,0xA0,0x23,0xCB,0x59,0x1D,0x1A,0x99,0x3C,0xFD};

  OneField() {
  }

  virtual ~OneField() throw() {}

  static apache::thrift::reflection::local::TypeSpec* local_reflection;

  EmptyStruct field;

  struct __isset {
    __isset() : field(false) {}
    bool field;
  } __isset;

  bool operator == (const OneField & rhs) const
  {
    if (!(field == rhs.field))
      return false;
    return true;
  }
  bool operator != (const OneField &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const OneField & ) const;

  uint32_t read(apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(apache::thrift::protocol::TProtocol* oprot) const;

};

// {1:{};}
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_2C32A565725135A023CB591D1A993CFD;

class VersioningTestV1 {
 public:

  static const char* ascii_fingerprint; // = "CB65C216C3190178AD7ADECF1928CD97";
  static const uint8_t binary_fingerprint[16]; // = {0xCB,0x65,0xC2,0x16,0xC3,0x19,0x01,0x78,0xAD,0x7A,0xDE,0xCF,0x19,0x28,0xCD,0x97};

  VersioningTestV1() : begin_in_both(0), old_string(""), end_in_both(0) {
  }

  virtual ~VersioningTestV1() throw() {}

  static apache::thrift::reflection::local::TypeSpec* local_reflection;

  int32_t begin_in_both;
  std::string old_string;
  int32_t end_in_both;

  struct __isset {
    __isset() : begin_in_both(false), old_string(false), end_in_both(false) {}
    bool begin_in_both;
    bool old_string;
    bool end_in_both;
  } __isset;

  bool operator == (const VersioningTestV1 & rhs) const
  {
    if (!(begin_in_both == rhs.begin_in_both))
      return false;
    if (!(old_string == rhs.old_string))
      return false;
    if (!(end_in_both == rhs.end_in_both))
      return false;
    return true;
  }
  bool operator != (const VersioningTestV1 &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const VersioningTestV1 & ) const;

  uint32_t read(apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(apache::thrift::protocol::TProtocol* oprot) const;

};

// {1:i32;3:string;12:i32;}
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_CB65C216C3190178AD7ADECF1928CD97;

class VersioningTestV2 {
 public:

  static const char* ascii_fingerprint; // = "085522DA9EF6E13EF3186B0B4C19DDDB";
  static const uint8_t binary_fingerprint[16]; // = {0x08,0x55,0x22,0xDA,0x9E,0xF6,0xE1,0x3E,0xF3,0x18,0x6B,0x0B,0x4C,0x19,0xDD,0xDB};

  VersioningTestV2() : begin_in_both(0), newint(0), newbyte(0), newshort(0), newlong(0), newdouble(0), newstring(""), end_in_both(0) {
  }

  virtual ~VersioningTestV2() throw() {}

  static apache::thrift::reflection::local::TypeSpec* local_reflection;

  int32_t begin_in_both;
  int32_t newint;
  int8_t newbyte;
  int16_t newshort;
  int64_t newlong;
  double newdouble;
  Bonk newstruct;
  std::vector<int32_t>  newlist;
  std::set<int32_t>  newset;
  std::map<int32_t, int32_t>  newmap;
  std::string newstring;
  int32_t end_in_both;

  struct __isset {
    __isset() : begin_in_both(false), newint(false), newbyte(false), newshort(false), newlong(false), newdouble(false), newstruct(false), newlist(false), newset(false), newmap(false), newstring(false), end_in_both(false) {}
    bool begin_in_both;
    bool newint;
    bool newbyte;
    bool newshort;
    bool newlong;
    bool newdouble;
    bool newstruct;
    bool newlist;
    bool newset;
    bool newmap;
    bool newstring;
    bool end_in_both;
  } __isset;

  bool operator == (const VersioningTestV2 & rhs) const
  {
    if (!(begin_in_both == rhs.begin_in_both))
      return false;
    if (!(newint == rhs.newint))
      return false;
    if (!(newbyte == rhs.newbyte))
      return false;
    if (!(newshort == rhs.newshort))
      return false;
    if (!(newlong == rhs.newlong))
      return false;
    if (!(newdouble == rhs.newdouble))
      return false;
    if (!(newstruct == rhs.newstruct))
      return false;
    if (!(newlist == rhs.newlist))
      return false;
    if (!(newset == rhs.newset))
      return false;
    if (!(newmap == rhs.newmap))
      return false;
    if (!(newstring == rhs.newstring))
      return false;
    if (!(end_in_both == rhs.end_in_both))
      return false;
    return true;
  }
  bool operator != (const VersioningTestV2 &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const VersioningTestV2 & ) const;

  uint32_t read(apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(apache::thrift::protocol::TProtocol* oprot) const;

};

// i16
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_2655F23011F3FE4C7C3D757D37D5F9A5;

// double
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_E8CD7DA078A86726031AD64F35F5A6C0;

// list<i32>
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_088BFAA0A90D491A87F1AA2810C1FC6D;

// map<i32,i32>
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_8E2C3AB94930BFC8CED12D3BE36D046C;

// {1:i32;2:i32;3:byte;4:i16;5:i64;6:double;7:{1:string;2:i32;};8:list<i32>;9:set<i32>;10:map<i32,i32>;11:string;12:i32;}
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_085522DA9EF6E13EF3186B0B4C19DDDB;

class ListTypeVersioningV1 {
 public:

  static const char* ascii_fingerprint; // = "CCCCE89C7E9DA10280F5663700677313";
  static const uint8_t binary_fingerprint[16]; // = {0xCC,0xCC,0xE8,0x9C,0x7E,0x9D,0xA1,0x02,0x80,0xF5,0x66,0x37,0x00,0x67,0x73,0x13};

  ListTypeVersioningV1() : hello("") {
  }

  virtual ~ListTypeVersioningV1() throw() {}

  static apache::thrift::reflection::local::TypeSpec* local_reflection;

  std::vector<int32_t>  myints;
  std::string hello;

  struct __isset {
    __isset() : myints(false), hello(false) {}
    bool myints;
    bool hello;
  } __isset;

  bool operator == (const ListTypeVersioningV1 & rhs) const
  {
    if (!(myints == rhs.myints))
      return false;
    if (!(hello == rhs.hello))
      return false;
    return true;
  }
  bool operator != (const ListTypeVersioningV1 &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ListTypeVersioningV1 & ) const;

  uint32_t read(apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(apache::thrift::protocol::TProtocol* oprot) const;

};

// {1:list<i32>;2:string;}
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_CCCCE89C7E9DA10280F5663700677313;

class ListTypeVersioningV2 {
 public:

  static const char* ascii_fingerprint; // = "BE556BF7091B2DABBA1863D5E458B15F";
  static const uint8_t binary_fingerprint[16]; // = {0xBE,0x55,0x6B,0xF7,0x09,0x1B,0x2D,0xAB,0xBA,0x18,0x63,0xD5,0xE4,0x58,0xB1,0x5F};

  ListTypeVersioningV2() : hello("") {
  }

  virtual ~ListTypeVersioningV2() throw() {}

  static apache::thrift::reflection::local::TypeSpec* local_reflection;

  std::vector<std::string>  strings;
  std::string hello;

  struct __isset {
    __isset() : strings(false), hello(false) {}
    bool strings;
    bool hello;
  } __isset;

  bool operator == (const ListTypeVersioningV2 & rhs) const
  {
    if (!(strings == rhs.strings))
      return false;
    if (!(hello == rhs.hello))
      return false;
    return true;
  }
  bool operator != (const ListTypeVersioningV2 &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const ListTypeVersioningV2 & ) const;

  uint32_t read(apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(apache::thrift::protocol::TProtocol* oprot) const;

};

// list<string>
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_1F997684D04C8586EA5BDBB510E47233;

// {1:list<string>;2:string;}
extern apache::thrift::reflection::local::TypeSpec
trlo_typespec_ThriftTest_BE556BF7091B2DABBA1863D5E458B15F;

}} // namespace

#endif
