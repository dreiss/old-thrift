// Copyright (c) 2006- Facebook
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/

#ifndef _THRIFT_TAPPLICATIONEXCEPTION_H_
#define _THRIFT_TAPPLICATIONEXCEPTION_H_ 1

#include <Thrift.h>


namespace facebook { namespace thrift {

namespace protocol {
  class TProtocol;
}

class TApplicationException : public TException {
 public:

  /**
   * Error codes for the various types of exceptions.
   */
  enum TApplicationExceptionType
  { UNKNOWN = 0
  , UNKNOWN_METHOD = 1
  , INVALID_MESSAGE_TYPE = 2
  , WRONG_METHOD_NAME = 3
  , BAD_SEQUENCE_ID = 4
  , MISSING_RESULT = 5
  };

  TApplicationException() :
    TException(),
    type_(UNKNOWN) {}

  TApplicationException(TApplicationExceptionType type) :
    TException(),
    type_(type) {}

  TApplicationException(const std::string& message) :
    TException(message),
    type_(UNKNOWN) {}

  TApplicationException(TApplicationExceptionType type,
                        const std::string& message) :
    TException(message),
    type_(type) {}

  virtual ~TApplicationException() throw() {}

  /**
   * Returns an error code that provides information about the type of error
   * that has occurred.
   *
   * @return Error code
   */
  TApplicationExceptionType getType() {
    return type_;
  }

  virtual const char* what() const throw() {
    if (message_.empty()) {
      switch (type_) {
        case UNKNOWN              : return "TApplicationException: Unknown application exception";
        case UNKNOWN_METHOD       : return "TApplicationException: Unknown method";
        case INVALID_MESSAGE_TYPE : return "TApplicationException: Invalid message type";
        case WRONG_METHOD_NAME    : return "TApplicationException: Wrong method name";
        case BAD_SEQUENCE_ID      : return "TApplicationException: Bad sequence identifier";
        case MISSING_RESULT       : return "TApplicationException: Missing result";
        default                   : return "TApplicationException: (Invalid exception type)";
      };
    } else {
      return message_.c_str();
    }
  }

  uint32_t read(protocol::TProtocol* iprot);
  uint32_t write(protocol::TProtocol* oprot) const;

 protected:
  /**
   * Error code
   */
  TApplicationExceptionType type_;

};

}} // facebook::thrift

#endif // #ifndef _THRIFT_TAPPLICATIONEXCEPTION_H_
