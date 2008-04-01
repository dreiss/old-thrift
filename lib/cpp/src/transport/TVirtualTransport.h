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
  virtual bool isOpen_virt() {
    return static_cast<Transport_*>(this)->isOpen();
  }

  virtual bool peek_virt() {
    return static_cast<Transport_*>(this)->peek();
  }

  virtual void open_virt() {
    static_cast<Transport_*>(this)->open();
  }

  virtual void close_virt() {
    static_cast<Transport_*>(this)->close();
  }

  virtual uint32_t read_virt(uint8_t* buf, uint32_t len) {
    printf("TVirtualTransport::read_virt: %s\n", __PRETTY_FUNCTION__);
    return static_cast<Transport_*>(this)->read(buf, len);
  }

  virtual uint32_t readAll_virt(uint8_t* buf, uint32_t len) {
    printf("TVirtualTransport::readAll_virt: %s\n", __PRETTY_FUNCTION__);
    return readAll(buf, len);
  }

  uint32_t readAll(uint8_t* buf, uint32_t len) {
    return facebook::thrift::transport::readAll(*static_cast<Transport_*>(this), buf, len);
  }

  virtual void readEnd_virt() {
    return static_cast<Transport_*>(this)->readEnd();
  }

  virtual void write_virt(const uint8_t* buf, uint32_t len) {
    static_cast<Transport_*>(this)->write(buf, len);
  }

  virtual void writeEnd_virt() {
    static_cast<Transport_*>(this)->writeEnd();
  }

  virtual void flush_virt() {
    static_cast<Transport_*>(this)->flush();
  }

  virtual const uint8_t* borrow_virt(uint8_t* buf, uint32_t* len) {
    return static_cast<Transport_*>(this)->borrow(buf, len);
  }

  virtual void consume(uint32_t len) {
    static_cast<Transport_*>(this)->consume(len);
  }

 protected:

  TVirtualTransport() {}
};

}}} // facebook::thrift::transport

#endif // #ifndef _THRIFT_TRANSPORT_TVIRTUALTRANSPORT_H_
