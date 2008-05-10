// Copyright (c) 2006- Facebook
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/

#ifndef _THRIFT_PROTOCOL_TVIRTUALPROTOCOL_H_
#define _THRIFT_PROTOCOL_TVIRTUALPROTOCOL_H_ 1

#include <protocol/TProtocol.h>

namespace facebook { namespace thrift { namespace protocol {

using facebook::thrift::transport::TTransport;


/**
 * Concrete TProtocol classes should inherit from TVirtualProtocol
 * so they don't have to manually override virtual methods.
 *
 * TODO(dreiss): Fix the infinite loop that shows up if someone doesn't
 * implement one of the protocol methods.  This is less of a problem than
 * with the transport classes because these are all pure-virtual (or at
 * least used to be).
 *
 * @author David Reiss <dreiss@facebook.com>
 */
template <class Protocol_>
class TVirtualProtocol : public TProtocol {
 public:
  /**
   * Writing functions.
   */

  virtual uint32_t writeMessageBegin_virtual(const std::string& name,
                                             const TMessageType messageType,
                                             const int32_t seqid) {
    return static_cast<Protocol_*>(this)->writeMessageBegin(name, messageType, seqid);
  }

  virtual uint32_t writeMessageEnd_virtual() {
    return static_cast<Protocol_*>(this)->writeMessageEnd();
  }


  virtual uint32_t writeStructBegin_virtual(const char* name) {
    return static_cast<Protocol_*>(this)->writeStructBegin(name);
  }

  virtual uint32_t writeStructEnd_virtual() {
    return static_cast<Protocol_*>(this)->writeStructEnd();
  }

  virtual uint32_t writeFieldBegin_virtual(const char* name,
                                           const TType fieldType,
                                           const int16_t fieldId) {
    return static_cast<Protocol_*>(this)->writeFieldBegin(name, fieldType, fieldId);
  }

  virtual uint32_t writeFieldEnd_virtual() {
    return static_cast<Protocol_*>(this)->writeFieldEnd();
  }

  virtual uint32_t writeFieldStop_virtual() {
    return static_cast<Protocol_*>(this)->writeFieldStop();
  }

  virtual uint32_t writeMapBegin_virtual(const TType keyType,
                                         const TType valType,
                                         const uint32_t size) {
    return static_cast<Protocol_*>(this)->writeMapBegin(keyType, valType, size);
  }

  virtual uint32_t writeMapEnd_virtual() {
    return static_cast<Protocol_*>(this)->writeMapEnd();
  }

  virtual uint32_t writeListBegin_virtual(const TType elemType,
                                          const uint32_t size) {
    return static_cast<Protocol_*>(this)->writeListBegin(elemType, size);
  }

  virtual uint32_t writeListEnd_virtual() {
    return static_cast<Protocol_*>(this)->writeListEnd();
  }

  virtual uint32_t writeSetBegin_virtual(const TType elemType,
                                         const uint32_t size) {
    return static_cast<Protocol_*>(this)->writeSetBegin(elemType, size);
  }

  virtual uint32_t writeSetEnd_virtual() {
    return static_cast<Protocol_*>(this)->writeSetEnd();
  }

  virtual uint32_t writeBool_virtual(const bool value) {
    return static_cast<Protocol_*>(this)->writeBool(value);
  }

  virtual uint32_t writeByte_virtual(const int8_t byte) {
    return static_cast<Protocol_*>(this)->writeByte(byte);
  }

  virtual uint32_t writeI16_virtual(const int16_t i16) {
    return static_cast<Protocol_*>(this)->writeI16(i16);
  }

  virtual uint32_t writeI32_virtual(const int32_t i32) {
    return static_cast<Protocol_*>(this)->writeI32(i32);
  }

  virtual uint32_t writeI64_virtual(const int64_t i64) {
    return static_cast<Protocol_*>(this)->writeI64(i64);
  }

  virtual uint32_t writeDouble_virtual(const double dub) {
    return static_cast<Protocol_*>(this)->writeDouble(dub);
  }

  virtual uint32_t writeString_virtual(const std::string& str) {
    return static_cast<Protocol_*>(this)->writeString(str);
  }

  virtual uint32_t writeBinary_virtual(const std::string& str) {
    return static_cast<Protocol_*>(this)->writeBinary(str);
  }

  /**
   * Reading functions
   */

  virtual uint32_t readMessageBegin_virtual(std::string& name,
                                            TMessageType& messageType,
                                            int32_t& seqid) {
    return static_cast<Protocol_*>(this)->readMessageBegin(name, messageType, seqid);
  }

  virtual uint32_t readMessageEnd_virtual() {
    return static_cast<Protocol_*>(this)->readMessageEnd();
  }

  virtual uint32_t readStructBegin_virtual(std::string& name) {
    return static_cast<Protocol_*>(this)->readStructBegin(name);
  }

  virtual uint32_t readStructEnd_virtual() {
    return static_cast<Protocol_*>(this)->readStructEnd();
  }

  virtual uint32_t readFieldBegin_virtual(std::string& name,
                                          TType& fieldType,
                                          int16_t& fieldId) {
    return static_cast<Protocol_*>(this)->readFieldBegin(name, fieldType, fieldId);
  }

  virtual uint32_t readFieldEnd_virtual() {
    return static_cast<Protocol_*>(this)->readFieldEnd();
  }

  virtual uint32_t readMapBegin_virtual(TType& keyType,
                                        TType& valType,
                                        uint32_t& size) {
    return static_cast<Protocol_*>(this)->readMapBegin(keyType, valType, size);
  }

  virtual uint32_t readMapEnd_virtual() {
    return static_cast<Protocol_*>(this)->readMapEnd();
  }

  virtual uint32_t readListBegin_virtual(TType& elemType,
                                         uint32_t& size) {
    return static_cast<Protocol_*>(this)->readListBegin(elemType, size);
  }

  virtual uint32_t readListEnd_virtual() {
    return static_cast<Protocol_*>(this)->readListEnd();
  }

  virtual uint32_t readSetBegin_virtual(TType& elemType,
                                        uint32_t& size) {
    return static_cast<Protocol_*>(this)->readSetBegin(elemType, size);
  }

  virtual uint32_t readSetEnd_virtual() {
    return static_cast<Protocol_*>(this)->readSetEnd();
  }

  virtual uint32_t readBool_virtual(bool& value) {
    return static_cast<Protocol_*>(this)->readBool(value);
  }

  virtual uint32_t readByte_virtual(int8_t& byte) {
    return static_cast<Protocol_*>(this)->readByte(byte);
  }

  virtual uint32_t readI16_virtual(int16_t& i16) {
    return static_cast<Protocol_*>(this)->readI16(i16);
  }

  virtual uint32_t readI32_virtual(int32_t& i32) {
    return static_cast<Protocol_*>(this)->readI32(i32);
  }

  virtual uint32_t readI64_virtual(int64_t& i64) {
    return static_cast<Protocol_*>(this)->readI64(i64);
  }

  virtual uint32_t readDouble_virtual(double& dub) {
    return static_cast<Protocol_*>(this)->readDouble(dub);
  }

  virtual uint32_t readString_virtual(std::string& str) {
    return static_cast<Protocol_*>(this)->readString(str);
  }

  virtual uint32_t readBinary_virtual(std::string& str) {
    return static_cast<Protocol_*>(this)->readBinary(str);
  }

 protected:
  TVirtualProtocol(boost::shared_ptr<TTransport> ptrans)
    : TProtocol(ptrans)
  {}
};

}}} // facebook::thrift::protocol

#endif // #define _THRIFT_PROTOCOL_TVIRTUALPROTOCOL_H_ 1
