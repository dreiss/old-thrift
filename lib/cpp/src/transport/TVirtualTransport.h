// Copyright (c) 2006- Facebook
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/

#ifndef _THRIFT_TRANSPORT_TVIRTUALTRANSPORT_H_
#define _THRIFT_TRANSPORT_TVIRTUALTRANSPORT_H_ 1

#include <transport/TTransport.h>

namespace facebook { namespace thrift { namespace transport {

/**
 * TODO
 *
 * @author Chad Walters <chad@powerst.com>
 */
template <class Transport_>
class TVirtualTransport : public TTransport {
 public:
  virtual uint32_t read_virt(uint8_t* buf, uint32_t len) {
    return static_cast<Transport_*>(this)->read(buf, len);
  }

  virtual uint32_t readAll_virt(uint8_t* buf, uint32_t len) {
    return readAll(buf, len);
  }

  uint32_t readAll(uint8_t* buf, uint32_t len) {
    return facebook::thrift::transport::readAll(*static_cast<Transport_*>(this), buf, len);
  }

  virtual void write_virt(const uint8_t* buf, uint32_t len) {
    static_cast<Transport_*>(this)->write(buf, len);
  }

  virtual const uint8_t* borrow_virt(uint8_t* buf, uint32_t* len) {
    return static_cast<Transport_*>(this)->borrow(buf, len);
  }

  virtual void consume_virt(uint32_t len) {
    static_cast<Transport_*>(this)->consume(len);
  }

  uint32_t read(uint8_t* buf, uint32_t len) {
    throw TTransportException(TTransportException::NOT_OPEN, "Base TTransport cannot read.");
  }
  void write(const uint8_t* buf, uint32_t len) {
    throw TTransportException(TTransportException::NOT_OPEN, "Base TTransport cannot write.");
  }
  const uint8_t* borrow(uint8_t* buf, uint32_t* len) {
    return NULL;
  }
  void consume(uint32_t len) {
    throw TTransportException(TTransportException::NOT_OPEN, "Base TTransport cannot consume.");
  }

 protected:
  TVirtualTransport() {}
};

}}} // facebook::thrift::transport

#endif // #ifndef _THRIFT_TRANSPORT_TVIRTUALTRANSPORT_H_
