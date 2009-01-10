// Copyright (c) 2006- Facebook
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/

#ifndef _THRIFT_TRANSPORT_TSIMPLEFILETRANSPORT_H_
#define _THRIFT_TRANSPORT_TSIMPLEFILETRANSPORT_H_ 1

#include "TFDTransport.h"

namespace facebook { namespace thrift { namespace transport {

/**
 * Dead-simple wrapper around a file.
 *
 * @author David Braginsky <dbraginsky@facebook.com>
 */
class TSimpleFileTransport : public TFDTransport {
 public:
  TSimpleFileTransport(std::string path, bool read, bool write);
};

}}} // facebook::thrift::transport

#endif //  _THRIFT_TRANSPORT_TSIMPLEFILETRANSPORT_H_
