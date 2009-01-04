// Copyright (c) 2006- Facebook
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/

#ifndef _THRIFT_TRANSPORT_THTTPSERVER_H_
#define _THRIFT_TRANSPORT_THTTPSERVER_H_ 1

#include <transport/THttpTransport.h>

namespace facebook { namespace thrift { namespace transport {

/**
 * HTTP server transport implementation.
 *
 * @author Jeremie BORDIER <jeremie.bordier@gmail.com>
 */
class THttpServer : public THttpTransport {
 public:
  THttpServer(boost::shared_ptr<TTransport> transport);

  virtual ~THttpServer();

  virtual void flush();

 protected:

  void readHeaders();
  virtual void parseHeader(char* header);
  virtual bool parseStatusLine(char* status);
  std::string getTimeRFC1123();

};

/**
 * Wraps a transport into HTTP protocol
 *
 * @author Jeremie BORDIER <jeremie.bordier@gmail.com>
 */
class THttpServerTransportFactory : public TTransportFactory {
 public:
  THttpServerTransportFactory() {}

  virtual ~THttpServerTransportFactory() {}

  /**
   * Wraps the transport into a buffered one.
   */
  virtual boost::shared_ptr<TTransport> getTransport(boost::shared_ptr<TTransport> trans) {
    return boost::shared_ptr<TTransport>(new THttpServer(trans));
  }

};

}}} // facebook::thrift::transport

#endif // #ifndef _THRIFT_TRANSPORT_THTTPSERVER_H_
