// Copyright (c) 2006- Facebook
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/

#ifndef _THRIFT_PROTOCOL_TPROTOCOL_H_
#define _THRIFT_PROTOCOL_TPROTOCOL_H_ 1

#include <transport/TTransport.h>
#include <protocol/TProtocolException.h>

#include <boost/shared_ptr.hpp>

#include <netinet/in.h>
#include <sys/types.h>
#include <string>
#include <map>

namespace facebook { namespace thrift { namespace protocol {

using facebook::thrift::transport::TTransport;

#ifdef HAVE_ENDIAN_H
#include <endian.h>
#endif

#ifndef __BYTE_ORDER
# if defined(BYTE_ORDER) && defined(LITTLE_ENDIAN) && defined(BIG_ENDIAN)
#  define __BYTE_ORDER BYTE_ORDER
#  define __LITTLE_ENDIAN LITTLE_ENDIAN
#  define __BIG_ENDIAN BIG_ENDIAN
# else
#  error "Cannot determine endianness"
# endif
#endif

#if __BYTE_ORDER == __BIG_ENDIAN
# define ntohll(n) (n)
# define htonll(n) (n)
#elif __BYTE_ORDER == __LITTLE_ENDIAN
# if defined(__GNUC__) && defined(__GLIBC__)
#  include <byteswap.h>
#  define ntohll(n) bswap_64(n)
#  define htonll(n) bswap_64(n)
# else /* GNUC & GLIBC */
#  define ntohll(n) ( (((unsigned long long)ntohl(n)) << 32) + ntohl(n >> 32) )
#  define htonll(n) ( (((unsigned long long)htonl(n)) << 32) + htonl(n >> 32) )
# endif /* GNUC & GLIBC */
#else /* __BYTE_ORDER */
# error "Can't define htonll or ntohll!"
#endif

/**
 * Enumerated definition of the types that the Thrift protocol supports.
 * Take special note of the T_END type which is used specifically to mark
 * the end of a sequence of fields.
 */
enum TType {
  T_STOP       = 0,
  T_VOID       = 1,
  T_BOOL       = 2,
  T_BYTE       = 3,
  T_I08        = 3,
  T_I16        = 6,
  T_I32        = 8,
  T_U64        = 9,
  T_I64        = 10,
  T_DOUBLE     = 4,
  T_STRING     = 11,
  T_UTF7       = 11,
  T_STRUCT     = 12,
  T_MAP        = 13,
  T_SET        = 14,
  T_LIST       = 15,
  T_UTF8       = 16,
  T_UTF16      = 17
};

/**
 * Enumerated definition of the message types that the Thrift protocol
 * supports.
 */
enum TMessageType {
  T_CALL       = 1,
  T_REPLY      = 2,
  T_EXCEPTION  = 3
};

/**
 * Abstract class for a thrift protocol driver. These are all the methods that
 * a protocol must implement. Essentially, there must be some way of reading
 * and writing all the base types, plus a mechanism for writing out structs
 * with indexed fields.
 *
 * TProtocol objects should not be shared across multiple encoding contexts,
 * as they may need to maintain internal state in some protocols (i.e. XML).
 * Note that is is acceptable for the TProtocol module to do its own internal
 * buffered reads/writes to the underlying TTransport where appropriate (i.e.
 * when parsing an input XML stream, reading should be batched rather than
 * looking ahead character by character for a close tag).
 *
 * @author Mark Slee <mcslee@facebook.com>
 */
class TProtocol {
 public:
  virtual ~TProtocol() {}

  /**
   * Writing functions.
   */

  virtual uint32_t writeMessageBegin_virtual(const std::string& name,
                                             const TMessageType messageType,
                                             const int32_t seqid) = 0;

  virtual uint32_t writeMessageEnd_virtual() = 0;


  virtual uint32_t writeStructBegin_virtual(const char* name) = 0;

  virtual uint32_t writeStructEnd_virtual() = 0;

  virtual uint32_t writeFieldBegin_virtual(const char* name,
                                           const TType fieldType,
                                           const int16_t fieldId) = 0;

  virtual uint32_t writeFieldEnd_virtual() = 0;

  virtual uint32_t writeFieldStop_virtual() = 0;

  virtual uint32_t writeMapBegin_virtual(const TType keyType,
                                         const TType valType,
                                         const uint32_t size) = 0;

  virtual uint32_t writeMapEnd_virtual() = 0;

  virtual uint32_t writeListBegin_virtual(const TType elemType,
                                          const uint32_t size) = 0;

  virtual uint32_t writeListEnd_virtual() = 0;

  virtual uint32_t writeSetBegin_virtual(const TType elemType,
                                         const uint32_t size) = 0;

  virtual uint32_t writeSetEnd_virtual() = 0;

  virtual uint32_t writeBool_virtual(const bool value) = 0;

  virtual uint32_t writeByte_virtual(const int8_t byte) = 0;

  virtual uint32_t writeI16_virtual(const int16_t i16) = 0;

  virtual uint32_t writeI32_virtual(const int32_t i32) = 0;

  virtual uint32_t writeI64_virtual(const int64_t i64) = 0;

  virtual uint32_t writeDouble_virtual(const double dub) = 0;

  virtual uint32_t writeString_virtual(const std::string& str) = 0;

  virtual uint32_t writeBinary_virtual(const std::string& str) = 0;

  uint32_t writeMessageBegin(const std::string& name,
                             const TMessageType messageType,
                             const int32_t seqid) {
    return writeMessageBegin_virtual(name, messageType, seqid);
  }

  uint32_t writeMessageEnd() {
    return writeMessageEnd_virtual();
  }


  uint32_t writeStructBegin(const char* name) {
    return writeStructBegin_virtual(name);
  }

  uint32_t writeStructEnd() {
    return writeStructEnd_virtual();
  }

  uint32_t writeFieldBegin(const char* name,
                           const TType fieldType,
                           const int16_t fieldId) {
    return writeFieldBegin_virtual(name, fieldType, fieldId);
  }

  uint32_t writeFieldEnd() {
    return writeFieldEnd_virtual();
  }

  uint32_t writeFieldStop() {
    return writeFieldStop_virtual();
  }

  uint32_t writeMapBegin(const TType keyType,
                         const TType valType,
                         const uint32_t size) {
    return writeMapBegin_virtual(keyType, valType, size);
  }

  uint32_t writeMapEnd() {
    return writeMapEnd_virtual();
  }

  uint32_t writeListBegin(const TType elemType,
                          const uint32_t size) {
    return writeListBegin_virtual(elemType, size);
  }

  uint32_t writeListEnd() {
    return writeListEnd_virtual();
  }

  uint32_t writeSetBegin(const TType elemType,
                         const uint32_t size) {
    return writeSetBegin_virtual(elemType, size);
  }

  uint32_t writeSetEnd() {
    return writeSetEnd_virtual();
  }

  uint32_t writeBool(const bool value) {
    return writeBool_virtual(value);
  }

  uint32_t writeByte(const int8_t byte) {
    return writeByte_virtual(byte);
  }

  uint32_t writeI16(const int16_t i16) {
    return writeI16_virtual(i16);
  }

  uint32_t writeI32(const int32_t i32) {
    return writeI32_virtual(i32);
  }

  uint32_t writeI64(const int64_t i64) {
    return writeI64_virtual(i64);
  }

  uint32_t writeDouble(const double dub) {
    return writeDouble_virtual(dub);
  }

  uint32_t writeString(const std::string& str) {
    return writeString_virtual(str);
  }

  uint32_t writeBinary(const std::string& str) {
    return writeBinary_virtual(str);
  }

  /**
   * Reading functions
   */

  virtual uint32_t readMessageBegin_virtual(std::string& name,
                                            TMessageType& messageType,
                                            int32_t& seqid) = 0;

  virtual uint32_t readMessageEnd_virtual() = 0;

  virtual uint32_t readStructBegin_virtual(std::string& name) = 0;

  virtual uint32_t readStructEnd_virtual() = 0;

  virtual uint32_t readFieldBegin_virtual(std::string& name,
                                          TType& fieldType,
                                          int16_t& fieldId) = 0;

  virtual uint32_t readFieldEnd_virtual() = 0;

  virtual uint32_t readMapBegin_virtual(TType& keyType,
                                        TType& valType,
                                        uint32_t& size) = 0;

  virtual uint32_t readMapEnd_virtual() = 0;

  virtual uint32_t readListBegin_virtual(TType& elemType,
                                         uint32_t& size) = 0;

  virtual uint32_t readListEnd_virtual() = 0;

  virtual uint32_t readSetBegin_virtual(TType& elemType,
                                        uint32_t& size) = 0;

  virtual uint32_t readSetEnd_virtual() = 0;

  virtual uint32_t readBool_virtual(bool& value) = 0;

  virtual uint32_t readByte_virtual(int8_t& byte) = 0;

  virtual uint32_t readI16_virtual(int16_t& i16) = 0;

  virtual uint32_t readI32_virtual(int32_t& i32) = 0;

  virtual uint32_t readI64_virtual(int64_t& i64) = 0;

  virtual uint32_t readDouble_virtual(double& dub) = 0;

  virtual uint32_t readString_virtual(std::string& str) = 0;

  virtual uint32_t readBinary_virtual(std::string& str) = 0;

  uint32_t readMessageBegin(std::string& name,
                            TMessageType& messageType,
                            int32_t& seqid) {
    return readMessageBegin_virtual(name, messageType, seqid);
  }

  uint32_t readMessageEnd() {
    return readMessageEnd_virtual();
  }

  uint32_t readStructBegin(std::string& name) {
    return readStructBegin_virtual(name);
  }

  uint32_t readStructEnd() {
    return readStructEnd_virtual();
  }

  uint32_t readFieldBegin(std::string& name,
                          TType& fieldType,
                          int16_t& fieldId) {
    return readFieldBegin_virtual(name, fieldType, fieldId);
  }

  uint32_t readFieldEnd() {
    return readFieldEnd_virtual();
  }

  uint32_t readMapBegin(TType& keyType,
                        TType& valType,
                        uint32_t& size) {
    return readMapBegin_virtual(keyType, valType, size);
  }

  uint32_t readMapEnd() {
    return readMapEnd_virtual();
  }

  uint32_t readListBegin(TType& elemType,
                         uint32_t& size) {
    return readListBegin_virtual(elemType, size);
  }

  uint32_t readListEnd() {
    return readListEnd_virtual();
  }

  uint32_t readSetBegin(TType& elemType,
                        uint32_t& size) {
    return readSetBegin_virtual(elemType, size);
  }

  uint32_t readSetEnd() {
    return readSetEnd_virtual();
  }

  uint32_t readBool(bool& value) {
    return readBool_virtual(value);
  }

  uint32_t readByte(int8_t& byte) {
    return readByte_virtual(byte);
  }

  uint32_t readI16(int16_t& i16) {
    return readI16_virtual(i16);
  }

  uint32_t readI32(int32_t& i32) {
    return readI32_virtual(i32);
  }

  uint32_t readI64(int64_t& i64) {
    return readI64_virtual(i64);
  }

  uint32_t readDouble(double& dub) {
    return readDouble_virtual(dub);
  }

  uint32_t readString(std::string& str) {
    return readString_virtual(str);
  }

  uint32_t readBinary(std::string& str) {
    return readBinary_virtual(str);
  }

  /**
   * Method to arbitrarily skip over data.
   * TODO(dreiss): Templatize this later?
   */
  uint32_t skip(TType type);

  inline boost::shared_ptr<TTransport> getTransport() {
    return ptrans_;
  }

  // TODO: remove these two calls, they are for backwards
  // compatibility
  inline boost::shared_ptr<TTransport> getInputTransport() {
    return ptrans_;
  }
  inline boost::shared_ptr<TTransport> getOutputTransport() {
    return ptrans_;
  }

 protected:
  TProtocol(boost::shared_ptr<TTransport> ptrans):
    ptrans_(ptrans) {
  }

  boost::shared_ptr<TTransport> ptrans_;

 private:
  TProtocol() {}
};


/**
 * Constructs input and output protocol objects given transports.
 */
class TProtocolFactory {
 public:
  TProtocolFactory() {}

  virtual ~TProtocolFactory() {}

  virtual boost::shared_ptr<TProtocol> getProtocol(boost::shared_ptr<TTransport> trans) = 0;
};

}}} // facebook::thrift::protocol

#endif // #define _THRIFT_PROTOCOL_TPROTOCOL_H_ 1
