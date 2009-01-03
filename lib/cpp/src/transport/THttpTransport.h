#ifndef _THRIFT_TRANSPORT_THTTPTRANSPORT_H_
#define _THRIFT_TRANSPORT_THTTPTRANSPORT_H_ 1

#include <transport/TBufferTransports.h>

namespace facebook { namespace thrift { namespace transport {

/**
 * HTTP Client/Server transport common base (interface), based on
 * Mark Slee's <mcslee@facebook.com> THttpClient implementation.
 *
 * His old comments are still valid: This was irritating
 * to write, but the alternatives in C++ land are daunting. Linking CURL
 * requires 23 dynamic libraries last time I checked (WTF?!?). All we have
 * here is a VERY basic HTTP/1.1 client which supports HTTP 100 Continue,
 * chunked transfer encoding, keepalive, etc. Tested against Apache.
 *
 * @author Jeremie BORDIER <jeremie.bordier@gmail.com>, Mark Slee <mcslee@facebook.com>
 */
class THttpTransport : public TTransport {
 public:
  THttpTransport(boost::shared_ptr<TTransport> transport);

  virtual ~THttpTransport();

  void open() {
    transport_->open();
  }

  bool isOpen() {
    return transport_->isOpen();
  }

  bool peek() {
    return transport_->peek();
  }

  void close() {
    transport_->close();
  }

  uint32_t read(uint8_t* buf, uint32_t len);

  void readEnd();

  void write(const uint8_t* buf, uint32_t len);

  virtual void flush() = 0;

 protected:

  boost::shared_ptr<TTransport> transport_;

  TMemoryBuffer writeBuffer_;
  TMemoryBuffer readBuffer_;

  bool readHeaders_;
  bool chunked_;
  bool chunkedDone_;
  uint32_t chunkSize_;
  uint32_t contentLength_;

  char* httpBuf_;
  uint32_t httpPos_;
  uint32_t httpBufLen_;
  uint32_t httpBufSize_;

  virtual void init();

  uint32_t readMoreData();
  char* readLine();

  void readHeaders();
  virtual void parseHeader(char* header) = 0;
  virtual bool parseStatusLine(char* status) = 0;

  uint32_t readChunked();
  void readChunkedFooters();
  uint32_t parseChunkSize(char* line);

  uint32_t readContent(uint32_t size);

  void refill();
  void shift();

  static const char* CRLF;
  static const int CRLF_LEN;
};

}}} // facebook::thrift::transport

#endif // #ifndef _THRIFT_TRANSPORT_THTTPCLIENT_H_
