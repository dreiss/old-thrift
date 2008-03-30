// Copyright (c) 2006- Facebook
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/

#ifndef _THRIFT_PROTOCOL_TBINARYPROTOCOL_H_
#define _THRIFT_PROTOCOL_TBINARYPROTOCOL_H_ 1

#include "TProtocol.h"

#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>

namespace facebook { namespace thrift { namespace protocol {

/**
 * The default binary protocol for thrift. Writes all data in a very basic
 * binary format, essentially just spitting out the raw bytes.
 *
 * @author Mark Slee <mcslee@facebook.com>
 */
template <class Transport_>
class TBinaryProtocolT : public TProtocol {
 protected:
  static const int32_t VERSION_MASK = 0xffff0000;
  static const int32_t VERSION_1 = 0x80010000;
  // VERSION_2 (0x80020000)  is taken by TDenseProtocol.

 public:
  TBinaryProtocolT(boost::shared_ptr<Transport_> trans) :
    TProtocol(trans),
    string_limit_(0),
    container_limit_(0),
    strict_read_(false),
    strict_write_(true),
    string_buf_(NULL),
    string_buf_size_(0) {}

  TBinaryProtocolT(boost::shared_ptr<Transport_> trans,
                   int32_t string_limit,
                   int32_t container_limit,
                   bool strict_read,
                   bool strict_write) :
    TProtocol(trans),
    string_limit_(string_limit),
    container_limit_(container_limit),
    strict_read_(strict_read),
    strict_write_(strict_write),
    string_buf_(NULL),
    string_buf_size_(0) {}

  ~TBinaryProtocolT() {
    if (string_buf_ != NULL) {
      std::free(string_buf_);
      string_buf_size_ = 0;
    }
  }

  void setStringSizeLimit(int32_t string_limit) {
    string_limit_ = string_limit;
  }

  void setContainerSizeLimit(int32_t container_limit) {
    container_limit_ = container_limit;
  }

  void setStrict(bool strict_read, bool strict_write) {
    strict_read_ = strict_read;
    strict_write_ = strict_write;
  }

  /**
   * Writing functions.
   */

  virtual uint32_t writeMessageBegin(const std::string& name,
                                     const TMessageType messageType,
                                     const int32_t seqid);

  virtual uint32_t writeMessageEnd();


  uint32_t writeStructBegin(const std::string& name);

  uint32_t writeStructEnd();

  uint32_t writeFieldBegin(const std::string& name,
                           const TType fieldType,
                           const int16_t fieldId);

  uint32_t writeFieldEnd();

  uint32_t writeFieldStop();

  uint32_t writeMapBegin(const TType keyType,
                         const TType valType,
                         const uint32_t size);

  uint32_t writeMapEnd();

  uint32_t writeListBegin(const TType elemType,
                          const uint32_t size);

  uint32_t writeListEnd();

  uint32_t writeSetBegin(const TType elemType,
                         const uint32_t size);

  uint32_t writeSetEnd();

  uint32_t writeBool(const bool value);

  uint32_t writeByte(const int8_t byte);

  uint32_t writeI16(const int16_t i16);

  uint32_t writeI32(const int32_t i32);

  uint32_t writeI64(const int64_t i64);

  uint32_t writeDouble(const double dub);

  uint32_t writeString(const std::string& str);

  uint32_t writeBinary(const std::string& str);

  /**
   * Reading functions
   */


  uint32_t readMessageBegin(std::string& name,
                            TMessageType& messageType,
                            int32_t& seqid);

  uint32_t readMessageEnd();

  uint32_t readStructBegin(std::string& name);

  uint32_t readStructEnd();

  uint32_t readFieldBegin(std::string& name,
                          TType& fieldType,
                          int16_t& fieldId);

  uint32_t readFieldEnd();

  uint32_t readMapBegin(TType& keyType,
                        TType& valType,
                        uint32_t& size);

  uint32_t readMapEnd();

  uint32_t readListBegin(TType& elemType,
                         uint32_t& size);

  uint32_t readListEnd();

  uint32_t readSetBegin(TType& elemType,
                        uint32_t& size);

  uint32_t readSetEnd();

  uint32_t readBool(bool& value);

  uint32_t readByte(int8_t& byte);

  uint32_t readI16(int16_t& i16);

  uint32_t readI32(int32_t& i32);

  uint32_t readI64(int64_t& i64);

  uint32_t readDouble(double& dub);

  uint32_t readString(std::string& str);

  uint32_t readBinary(std::string& str);

 protected:
  uint32_t readStringBody(std::string& str, int32_t sz);

  int32_t string_limit_;
  int32_t container_limit_;

  // Enforce presence of version identifier
  bool strict_read_;
  bool strict_write_;

  // Buffer for reading strings, save for the lifetime of the protocol to
  // avoid memory churn allocating memory on every string read
  uint8_t* string_buf_;
  int32_t string_buf_size_;

};

typedef TBinaryProtocolT<TTransport> TBinaryProtocol;

/**
 * Constructs binary protocol handlers
 */
class TBinaryProtocolFactory : public TProtocolFactory {
 public:
  TBinaryProtocolFactory() :
    string_limit_(0),
    container_limit_(0),
    strict_read_(false),
    strict_write_(true) {}

  TBinaryProtocolFactory(int32_t string_limit, int32_t container_limit, bool strict_read, bool strict_write) :
    string_limit_(string_limit),
    container_limit_(container_limit),
    strict_read_(strict_read),
    strict_write_(strict_write) {}

  virtual ~TBinaryProtocolFactory() {}

  void setStringSizeLimit(int32_t string_limit) {
    string_limit_ = string_limit;
  }

  void setContainerSizeLimit(int32_t container_limit) {
    container_limit_ = container_limit;
  }

  void setStrict(bool strict_read, bool strict_write) {
    strict_read_ = strict_read;
    strict_write_ = strict_write;
  }

  boost::shared_ptr<TProtocol> getProtocol(boost::shared_ptr<TTransport> trans) {
    return boost::shared_ptr<TProtocol>(new TBinaryProtocol(trans, string_limit_, container_limit_, strict_read_, strict_write_));
  }

 private:
  int32_t string_limit_;
  int32_t container_limit_;
  bool strict_read_;
  bool strict_write_;

};

namespace details {

// Use this to get around strict aliasing rules.
// For example, uint64_t i = bitwise_cast<uint64_t>(returns_double());
// The most obvious implementation is to just cast a pointer,
// but that doesn't work.
// For a pretty in-depth explanation of the problem, see
// http://www.cellperformance.com/mike_acton/2006/06/ (...)
// understanding_strict_aliasing.html
template <typename To, typename From>
static inline To bitwise_cast(From from) {
  BOOST_STATIC_ASSERT(sizeof(From) == sizeof(To));

  // BAD!!!  These are all broken with -O2.
  //return *reinterpret_cast<To*>(&from);  // BAD!!!
  //return *static_cast<To*>(static_cast<void*>(&from));  // BAD!!!
  //return *(To*)(void*)&from;  // BAD!!!

  // Super clean and paritally blessed by section 3.9 of the standard.
  //unsigned char c[sizeof(from)];
  //memcpy(c, &from, sizeof(from));
  //To to;
  //memcpy(&to, c, sizeof(c));
  //return to;

  // Slightly more questionable.
  // Same code emitted by GCC.
  //To to;
  //memcpy(&to, &from, sizeof(from));
  //return to;

  // Technically undefined, but almost universally supported,
  // and the most efficient implementation.
  union {
    From f;
    To t;
  } u;
  u.f = from;
  return u.t;
}

}


template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeMessageBegin(const std::string& name,
                                                         const TMessageType messageType,
                                                         const int32_t seqid) {
  if (strict_write_) {
    int32_t version = (VERSION_1) | ((int32_t)messageType);
    uint32_t wsize = 0;
    wsize += writeI32(version);
    wsize += writeString(name);
    wsize += writeI32(seqid);
    return wsize;
  } else {
    uint32_t wsize = 0;
    wsize += writeString(name);
    wsize += writeByte((int8_t)messageType);
    wsize += writeI32(seqid);
    return wsize;
  }
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeMessageEnd() {
  return 0;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeStructBegin(const std::string& name) {
  return 0;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeStructEnd() {
  return 0;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeFieldBegin(const std::string& name,
                                                       const TType fieldType,
                                                       const int16_t fieldId) {
  uint32_t wsize = 0;
  wsize += writeByte((int8_t)fieldType);
  wsize += writeI16(fieldId);
  return wsize;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeFieldEnd() {
  return 0;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeFieldStop() {
  return
    writeByte((int8_t)T_STOP);
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeMapBegin(const TType keyType,
                                                     const TType valType,
                                                     const uint32_t size) {
  uint32_t wsize = 0;
  wsize += writeByte((int8_t)keyType);
  wsize += writeByte((int8_t)valType);
  wsize += writeI32((int32_t)size);
  return wsize;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeMapEnd() {
  return 0;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeListBegin(const TType elemType,
                                                      const uint32_t size) {
  uint32_t wsize = 0;
  wsize += writeByte((int8_t) elemType);
  wsize += writeI32((int32_t)size);
  return wsize;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeListEnd() {
  return 0;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeSetBegin(const TType elemType,
                                                     const uint32_t size) {
  uint32_t wsize = 0;
  wsize += writeByte((int8_t)elemType);
  wsize += writeI32((int32_t)size);
  return wsize;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeSetEnd() {
  return 0;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeBool(const bool value) {
  uint8_t tmp =  value ? 1 : 0;
  this->trans_->write(&tmp, 1);
  return 1;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeByte(const int8_t byte) {
  this->trans_->write((uint8_t*)&byte, 1);
  return 1;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeI16(const int16_t i16) {
  int16_t net = (int16_t)htons(i16);
  this->trans_->write((uint8_t*)&net, 2);
  return 2;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeI32(const int32_t i32) {
  int32_t net = (int32_t)htonl(i32);
  this->trans_->write((uint8_t*)&net, 4);
  return 4;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeI64(const int64_t i64) {
  int64_t net = (int64_t)htonll(i64);
  this->trans_->write((uint8_t*)&net, 8);
  return 8;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeDouble(const double dub) {
  BOOST_STATIC_ASSERT(sizeof(double) == sizeof(uint64_t));
  BOOST_STATIC_ASSERT(std::numeric_limits<double>::is_iec559);

  uint64_t bits = details::bitwise_cast<uint64_t>(dub);
  bits = htonll(bits);
  this->trans_->write((uint8_t*)&bits, 8);
  return 8;
}


template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeString(const std::string& str) {
  uint32_t size = str.size();
  uint32_t result = writeI32((int32_t)size);
  if (size > 0) {
    this->trans_->write((uint8_t*)str.data(), size);
  }
  return result + size;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::writeBinary(const std::string& str) {
  return TBinaryProtocolT<Transport_>::writeString(str);
}

/**
 * Reading functions
 */

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readMessageBegin(std::string& name,
                                                        TMessageType& messageType,
                                                        int32_t& seqid) {
  uint32_t result = 0;
  int32_t sz;
  result += readI32(sz);

  if (sz < 0) {
    // Check for correct version number
    int32_t version = sz & VERSION_MASK;
    if (version != VERSION_1) {
      throw TProtocolException(TProtocolException::BAD_VERSION, "Bad version identifier");
    }
    messageType = (TMessageType)(sz & 0x000000ff);
    result += readString(name);
    result += readI32(seqid);
  } else {
    if (strict_read_) {
      throw TProtocolException(TProtocolException::BAD_VERSION, "No version identifier... old protocol client in strict mode?");
    } else {
      // Handle pre-versioned input
      int8_t type;
      result += readStringBody(name, sz);
      result += readByte(type);
      messageType = (TMessageType)type;
      result += readI32(seqid);
    }
  }
  return result;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readMessageEnd() {
  return 0;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readStructBegin(std::string& name) {
  name = "";
  return 0;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readStructEnd() {
  return 0;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readFieldBegin(std::string& name,
                                                      TType& fieldType,
                                                      int16_t& fieldId) {
  uint32_t result = 0;
  int8_t type;
  result += readByte(type);
  fieldType = (TType)type;
  if (fieldType == T_STOP) {
    fieldId = 0;
    return result;
  }
  result += readI16(fieldId);
  return result;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readFieldEnd() {
  return 0;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readMapBegin(TType& keyType,
                                                    TType& valType,
                                                    uint32_t& size) {
  int8_t k, v;
  uint32_t result = 0;
  int32_t sizei;
  result += readByte(k);
  keyType = (TType)k;
  result += readByte(v);
  valType = (TType)v;
  result += readI32(sizei);
  if (sizei < 0) {
    throw TProtocolException(TProtocolException::NEGATIVE_SIZE);
  } else if (container_limit_ && sizei > container_limit_) {
    throw TProtocolException(TProtocolException::SIZE_LIMIT);
  }
  size = (uint32_t)sizei;
  return result;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readMapEnd() {
  return 0;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readListBegin(TType& elemType,
                                                     uint32_t& size) {
  int8_t e;
  uint32_t result = 0;
  int32_t sizei;
  result += readByte(e);
  elemType = (TType)e;
  result += readI32(sizei);
  if (sizei < 0) {
    throw TProtocolException(TProtocolException::NEGATIVE_SIZE);
  } else if (container_limit_ && sizei > container_limit_) {
    throw TProtocolException(TProtocolException::SIZE_LIMIT);
  }
  size = (uint32_t)sizei;
  return result;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readListEnd() {
  return 0;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readSetBegin(TType& elemType,
                                                    uint32_t& size) {
  int8_t e;
  uint32_t result = 0;
  int32_t sizei;
  result += readByte(e);
  elemType = (TType)e;
  result += readI32(sizei);
  if (sizei < 0) {
    throw TProtocolException(TProtocolException::NEGATIVE_SIZE);
  } else if (container_limit_ && sizei > container_limit_) {
    throw TProtocolException(TProtocolException::SIZE_LIMIT);
  }
  size = (uint32_t)sizei;
  return result;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readSetEnd() {
  return 0;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readBool(bool& value) {
  uint8_t b[1];
  this->trans_->readAll(b, 1);
  value = *(int8_t*)b != 0;
  return 1;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readByte(int8_t& byte) {
  uint8_t b[1];
  this->trans_->readAll(b, 1);
  byte = *(int8_t*)b;
  return 1;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readI16(int16_t& i16) {
  uint8_t b[2];
  this->trans_->readAll(b, 2);
  i16 = *(int16_t*)b;
  i16 = (int16_t)ntohs(i16);
  return 2;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readI32(int32_t& i32) {
  uint8_t b[4];
  this->trans_->readAll(b, 4);
  i32 = *(int32_t*)b;
  i32 = (int32_t)ntohl(i32);
  return 4;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readI64(int64_t& i64) {
  uint8_t b[8];
  this->trans_->readAll(b, 8);
  i64 = *(int64_t*)b;
  i64 = (int64_t)ntohll(i64);
  return 8;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readDouble(double& dub) {
  BOOST_STATIC_ASSERT(sizeof(double) == sizeof(uint64_t));
  BOOST_STATIC_ASSERT(std::numeric_limits<double>::is_iec559);

  uint64_t bits;
  uint8_t b[8];
  this->trans_->readAll(b, 8);
  bits = *(uint64_t*)b;
  bits = ntohll(bits);
  dub = details::bitwise_cast<double>(bits);
  return 8;
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readString(std::string& str) {
  uint32_t result;
  int32_t size;
  result = readI32(size);
  return result + readStringBody(str, size);
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readBinary(std::string& str) {
  return TBinaryProtocolT<Transport_>::readString(str);
}

template <class Transport_>
uint32_t TBinaryProtocolT<Transport_>::readStringBody(std::string& str, int32_t size) {
  uint32_t result = 0;

  // Catch error cases
  if (size < 0) {
    throw TProtocolException(TProtocolException::NEGATIVE_SIZE);
  }
  if (string_limit_ > 0 && size > string_limit_) {
    throw TProtocolException(TProtocolException::SIZE_LIMIT);
  }

  // Catch empty string case
  if (size == 0) {
    str = "";
    return result;
  }

  // Use the heap here to prevent stack overflow for v. large strings
  if (size > string_buf_size_ || string_buf_ == NULL) {
    string_buf_ = (uint8_t*)std::realloc(string_buf_, (uint32_t)size);
    if (string_buf_ == NULL) {
      string_buf_size_ = 0;
      throw TProtocolException(TProtocolException::UNKNOWN, "Out of memory in TBinaryProtocol::readString");
    }
    string_buf_size_ = size;
  }
  this->trans_->readAll(string_buf_, size);
  str = std::string((char*)string_buf_, size);
  return (uint32_t)size;
}

}}} // facebook::thrift::protocol

#endif // #ifndef _THRIFT_PROTOCOL_TBINARYPROTOCOL_H_
