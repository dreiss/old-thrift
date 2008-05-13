// Copyright (c) 2006- Facebook
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/

#ifndef _THRIFT_PROTOCOL_TONEWAYPROTOCOL_H_
#define _THRIFT_PROTOCOL_TONEWAYPROTOCOL_H_ 1

#include "TProtocol.h"
#include "TVirtualProtocol.h"

namespace facebook { namespace thrift { namespace protocol {

/**
 * Abstract class for implementing a protocol that can only be written,
 * not read.
 *
 * @author David Reiss <dreiss@facebook.com>
 */
template <class Protocol_>
class TWriteOnlyProtocol : public TVirtualProtocol<Protocol_> {
 public:
  /**
   * @param subclass_name  The name of the concrete subclass.
   */
  TWriteOnlyProtocol(boost::shared_ptr<TTransport> trans,
                     const std::string& subclass_name)
    : TVirtualProtocol<Protocol_>(trans)
    , trans_(trans.get())
    , subclass_(subclass_name)
  {}

  // All writing functions remain abstract.

  /**
   * Reading functions all throw an exception.
   */

  uint32_t readMessageBegin_virt(std::string& name,
                                 TMessageType& messageType,
                                 int32_t& seqid) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support reading (yet).");
  }

  uint32_t readMessageEnd_virt() {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support reading (yet).");
  }

  uint32_t readStructBegin_virt(std::string& name) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support reading (yet).");
  }

  uint32_t readStructEnd_virt() {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support reading (yet).");
  }

  uint32_t readFieldBegin_virt(std::string& name,
                               TType& fieldType,
                               int16_t& fieldId) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support reading (yet).");
  }

  uint32_t readFieldEnd_virt() {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support reading (yet).");
  }

  uint32_t readMapBegin_virt(TType& keyType,
                             TType& valType,
                             uint32_t& size) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support reading (yet).");
  }

  uint32_t readMapEnd_virt() {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support reading (yet).");
  }

  uint32_t readListBegin_virt(TType& elemType,
                              uint32_t& size) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support reading (yet).");
  }

  uint32_t readListEnd_virt() {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support reading (yet).");
  }

  uint32_t readSetBegin_virt(TType& elemType,
                             uint32_t& size) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support reading (yet).");
  }

  uint32_t readSetEnd_virt() {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support reading (yet).");
  }

  uint32_t readBool_virt(bool& value) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support reading (yet).");
  }

  uint32_t readByte_virt(int8_t& byte) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support reading (yet).");
  }

  uint32_t readI16_virt(int16_t& i16) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support reading (yet).");
  }

  uint32_t readI32_virt(int32_t& i32) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support reading (yet).");
  }

  uint32_t readI64_virt(int64_t& i64) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support reading (yet).");
  }

  uint32_t readDouble_virt(double& dub) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support reading (yet).");
  }

  uint32_t readString_virt(std::string& str) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support reading (yet).");
  }

  uint32_t readBinary_virt(std::string& str) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support reading (yet).");
  }

 protected:
  TTransport *trans_;
 private:
  std::string subclass_;
};


/**
 * Abstract class for implementing a protocol that can only be read,
 * not written.
 *
 * @author David Reiss <dreiss@facebook.com>
 */
template <class Protocol_>
class TReadOnlyProtocol : public TVirtualProtocol<Protocol_> {
 public:
  /**
   * @param subclass_name  The name of the concrete subclass.
   */
   TReadOnlyProtocol(boost::shared_ptr<TTransport> trans,
                     const std::string& subclass_name)
    : TVirtualProtocol<Protocol_>(trans)
    , subclass_(subclass_name)
  {}

  // All reading functions remain abstract.

  /**
   * Writing functions all throw an exception.
   */

  uint32_t writeMessageBegin_virt(const std::string& name,
                                  const TMessageType messageType,
                                  const int32_t seqid) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }

  uint32_t writeMessageEnd_virt() {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }


  uint32_t writeStructBegin_virt(const char* name) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }

  uint32_t writeStructEnd_virt() {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }

  uint32_t writeFieldBegin_virt(const char* name,
                                const TType fieldType,
                                const int16_t fieldId) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }

  uint32_t writeFieldEnd_virt() {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }

  uint32_t writeFieldStop_virt() {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }

  uint32_t writeMapBegin_virt(const TType keyType,
                              const TType valType,
                              const uint32_t size) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }

  uint32_t writeMapEnd_virt() {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }

  uint32_t writeListBegin_virt(const TType elemType,
                               const uint32_t size) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }

  uint32_t writeListEnd_virt() {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }

  uint32_t writeSetBegin_virt(const TType elemType,
                              const uint32_t size) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }

  uint32_t writeSetEnd_virt() {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }

  uint32_t writeBool_virt(const bool value) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }

  uint32_t writeByte_virt(const int8_t byte) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }

  uint32_t writeI16_virt(const int16_t i16) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }

  uint32_t writeI32_virt(const int32_t i32) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }

  uint32_t writeI64_virt(const int64_t i64) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }

  uint32_t writeDouble_virt(const double dub) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }

  uint32_t writeString_virt(const std::string& str) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }

  uint32_t writeBinary_virt(const std::string& str) {
    throw TProtocolException(TProtocolException::NOT_IMPLEMENTED,
        subclass_ + " does not support writing (yet).");
  }

 private:
  std::string subclass_;
};

}}} // facebook::thrift::protocol

#endif // #ifndef _THRIFT_PROTOCOL_TBINARYPROTOCOL_H_
