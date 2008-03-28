// Copyright (c) 2006- Facebook
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/

#ifndef _THRIFT_THRIFT_H_
#define _THRIFT_THRIFT_H_ 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <netinet/in.h>
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#include <string>
#include <map>
#include <list>
#include <set>
#include <vector>
#include <exception>

#include "TLogging.h"

namespace facebook { namespace thrift {

class TOutput {
 public:
  TOutput() : f_(&perrorTimeWrapper) {}

  inline void setOutputFunction(void (*function)(const char *)){
    f_ = function;
  }

  inline void operator()(const char *message){
    f_(message);
  }

  inline static void perrorTimeWrapper(const char* msg) {
    time_t now;
    char dbgtime[25];
    time(&now);
    ctime_r(&now, dbgtime);
    dbgtime[24] = 0;
    fprintf(stderr, "%s ", dbgtime);
    perror(msg);
  }
 private:
  void (*f_)(const char *);
};

extern TOutput GlobalOutput;

class TException : public std::exception {
 public:
  TException() {}

  TException(const std::string& message) :
    message_(message) {}

  virtual ~TException() throw() {}

  virtual const char* what() const throw() {
    if (message_.empty()) {
      return "Default TException.";
    } else {
      return message_.c_str();
    }
  }

 protected:
  std::string message_;

};

class TApplicationException;


// Forward declare this structure used by TDenseProtocol
namespace reflection { namespace local {
struct TypeSpec;
}}


}} // facebook::thrift

#endif // #ifndef _THRIFT_THRIFT_H_
