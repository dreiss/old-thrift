#include "TProtocol.h"

namespace facebook { namespace thrift { namespace protocol {
using namespace facebook::thrift::concurrency;
using facebook::thrift::transport::TTransport;

TProtocol::TProtocol(boost::shared_ptr<TTransport> ptrans):
  ptrans_(ptrans), mutex_(), seqid_(1) {
  trans_ = ptrans.get();
}

int32_t TProtocol::get_new_seqid()
{
  Guard g(mutex_);
  seqid_ += 1;
  return seqid_;
}
}}}
