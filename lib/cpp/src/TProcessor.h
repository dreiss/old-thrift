// Copyright (c) 2006- Facebook
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/

#ifndef _THRIFT_TPROCESSOR_H_
#define _THRIFT_TPROCESSOR_H_ 1

#include <string>
#include <protocol/TProtocol.h>
#include <boost/shared_ptr.hpp>

namespace facebook { namespace thrift {

/**
 * A processor is a generic object that acts upon two streams of data, one
 * an input and the other an output. The definition of this object is loose,
 * though the typical case is for some sort of server that either generates
 * responses to an input stream or forwards data from one pipe onto another.
 *
 * @author Mark Slee <mcslee@facebook.com>
 */
class TProcessor {
 public:
  virtual ~TProcessor() {}

  virtual bool process(boost::shared_ptr<protocol::TProtocol> in,
                       boost::shared_ptr<protocol::TProtocol> out) = 0;

  bool process(boost::shared_ptr<facebook::thrift::protocol::TProtocol> io) {
    return process(io, io);
  }

 protected:
  TProcessor() {}
};

/**
 * A TCobProcessor is like a TProcessor, but the server cannot assume that
 * the processing is done when process returns.  Instead, it passes a
 * Continuation OBject as a parameter.  The processor must call this object
 * when it is finished.
 *
 * @author David Reiss <dreiss@facebook.com>
 */
class TCobProcessor {
 public:
  virtual ~TCobProcessor() {}

  virtual void process(std::tr1::function<void(bool success)> cob,
                       boost::shared_ptr<protocol::TProtocol> in,
                       boost::shared_ptr<protocol::TProtocol> out) = 0;

  bool process(std::tr1::function<void(bool success)> cob,
               boost::shared_ptr<facebook::thrift::protocol::TProtocol> io) {
    return process(cob, io, io);
  }

 protected:
  TCobProcessor() {}
};

}} // facebook::thrift

#endif // #ifndef _THRIFT_PROCESSOR_H_
