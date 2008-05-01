// Copyright (c) 2006- Facebook
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/

#ifndef _THRIFT_TRANSPORT_TVIRTUALTRANSPORT_H_
#define _THRIFT_TRANSPORT_TVIRTUALTRANSPORT_H_ 1

#include <transport/TTransport.h>
#include <boost/type_traits.hpp>

namespace facebook { namespace thrift { namespace transport {

namespace stats {

enum Stat {
  STAT_IS_OPEN = 0,
  STAT_PEEK,
  STAT_OPEN,
  STAT_CLOSE,
  STAT_READ,
  STAT_READ_ALL,
  STAT_WRITE,
  STAT_READ_END,
  STAT_WRITE_END,
  STAT_FLUSH,
  STAT_BORROW,
  STAT_CONSUME,
  STAT_LAST = STAT_CONSUME  // N.B.: Must be equal to the last stat
};

// Allow for selection of real stats collector or null stats collector
template <typename T_>
struct StatsCollector;

template < >
struct StatsCollector<boost::true_type> {
  // Stats collector implementation

  StatsCollector<boost::true_type>() {
    clearAll();
  }

  int64_t get(const Stat stat) {
    return stats_[stat];
  }

  int64_t increment(const Stat stat) {
    return ++stats_[stat];
  }

  void clear (const Stat stat) {
    stats_[stat] = 0;
  }

  void clearAll() {
    memset(stats_, 0, sizeof(stats_));
  }

  bool isAllClear() {
    int64_t *cur = stats_;
    int64_t *end = stats_ + (STAT_LAST + 1);
    while (!*cur++) {
      if (cur == end) return true;
    }
    return false;
  }

 private:
  int64_t stats_[STAT_LAST + 1];
};

template < >
struct StatsCollector<boost::false_type> {
  // Null stats collector object -- compiles away to nothing

  int64_t get(const Stat /*stat*/) {
    // Do nothing
    return 0;
  }

  int64_t increment(const Stat /*stat*/) {
    // Do nothing
    return 0;
  }

  void clear (const Stat /*stat*/) {
    // Do nothing
  }

  void clearAll() {
    // Do nothing
  }

  bool isAllClear() {
    return false;
  }
};

} // namespace stats

/**
 * A virtual base template that provides virtual function support for transport
 * subclasses. Transport_ is the transport subclass which is expected to
 * inherit from TVirtualTransport (CRTP). CollectStats_ is a compile-time
 * boolean, indicating whether TVirtualTransport should tally virtual calls --
 * use boost::false_type for a null implementation (for production purposes) 
 * and boost::true_type for a working implementation (for testing purposes).
 *
 * @author Chad Walters <chad@powerst.com>
 */
template <class Transport_, typename CollectStats_ = boost::false_type>
class TVirtualTransport : public TTransport {
 public:
  virtual bool isOpen_virt() {
    stats_.increment(stats::STAT_IS_OPEN);
    return static_cast<Transport_*>(this)->isOpen();
  }

  virtual bool peek_virt() {
    stats_.increment(stats::STAT_PEEK);
    return static_cast<Transport_*>(this)->peek();
  }

  virtual void open_virt() {
    stats_.increment(stats::STAT_OPEN);
    static_cast<Transport_*>(this)->open();
  }

  virtual void close_virt() {
    stats_.increment(stats::STAT_CLOSE);
    static_cast<Transport_*>(this)->close();
  }

  virtual uint32_t read_virt(uint8_t* buf, uint32_t len) {
    stats_.increment(stats::STAT_READ);
    return static_cast<Transport_*>(this)->read(buf, len);
  }

  virtual uint32_t readAll_virt(uint8_t* buf, uint32_t len) {
    stats_.increment(stats::STAT_READ_ALL);
    return readAll(buf, len);
  }

  uint32_t readAll(uint8_t* buf, uint32_t len) {
    return facebook::thrift::transport::readAll(*static_cast<Transport_*>(this), buf, len);
  }

  virtual void readEnd_virt() {
    stats_.increment(stats::STAT_READ_END);
    return static_cast<Transport_*>(this)->readEnd();
  }

  virtual void write_virt(const uint8_t* buf, uint32_t len) {
    stats_.increment(stats::STAT_WRITE);
    static_cast<Transport_*>(this)->write(buf, len);
  }

  virtual void writeEnd_virt() {
    stats_.increment(stats::STAT_WRITE_END);
    static_cast<Transport_*>(this)->writeEnd();
  }

  virtual void flush_virt() {
    stats_.increment(stats::STAT_FLUSH);
    static_cast<Transport_*>(this)->flush();
  }

  virtual const uint8_t* borrow_virt(uint8_t* buf, uint32_t* len) {
    stats_.increment(stats::STAT_BORROW);
    return static_cast<Transport_*>(this)->borrow(buf, len);
  }

  virtual void consume_virt(uint32_t len) {
    stats_.increment(stats::STAT_CONSUME);
    static_cast<Transport_*>(this)->consume(len);
  }

  bool isOpen() {
    return false;
  }
  bool peek() {
    return isOpen_virt();
  }
  void open() {
    throw TTransportException(TTransportException::NOT_OPEN, "Cannot open base TTransport.");
  }
  void close() {
    throw TTransportException(TTransportException::NOT_OPEN, "Cannot close base TTransport.");
  }
  uint32_t read(uint8_t* buf, uint32_t len) {
    throw TTransportException(TTransportException::NOT_OPEN, "Base TTransport cannot read.");
  }
  void write(const uint8_t* buf, uint32_t len) {
    throw TTransportException(TTransportException::NOT_OPEN, "Base TTransport cannot write.");
  }
  void readEnd() {}
  void writeEnd() {}
  virtual void flush() {}
  const uint8_t* borrow(uint8_t* buf, uint32_t* len) {
    return NULL;
  }
  void consume(uint32_t len) {
    throw TTransportException(TTransportException::NOT_OPEN, "Base TTransport cannot consume.");
  }

  stats::StatsCollector<CollectStats_> &getStats(void) {
    return stats_;
  }

 protected:
  TVirtualTransport() {}
  virtual ~TVirtualTransport() {}

 private:
   stats::StatsCollector<CollectStats_> stats_;
};

}}} // facebook::thrift::transport

#endif // #ifndef _THRIFT_TRANSPORT_TVIRTUALTRANSPORT_H_
