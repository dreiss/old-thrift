// Copyright (c) 2006- Facebook
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/

#ifndef _THRIFT_TRANSPORT_THTTPCLIENT_H_
#define _THRIFT_TRANSPORT_THTTPCLIENT_H_ 1

#include <transport/THttpTransport.h>

namespace apache { namespace thrift { namespace transport {

/**
 * HTTP client implementation.
 *
 * @author Jeremie BORDIER <jeremie.bordier@gmail.com>
 */
class THttpClient : public THttpTransport {
 public:
  THttpClient(boost::shared_ptr<TTransport> transport, std::string host, std::string path="");

  virtual ~THttpClient();

  virtual void flush();

 protected:

  std::string host_;
  std::string path_;

  virtual void parseHeader(char* header);
  virtual bool parseStatusLine(char* status);

};

}}} // apache::thrift::transport

#endif // #ifndef _THRIFT_TRANSPORT_THTTPCLIENT_H_
