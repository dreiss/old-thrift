#include <iostream>
#include <climits>
#include <cassert>
#include <boost/shared_ptr.hpp>
#include <transport/TBufferTransports.h>
#include <protocol/TBinaryProtocol.h>
#include <TAny.h>
#include "anytest_types.h"

using facebook::thrift::any_cast;
using facebook::thrift::transport::TMemoryBuffer;
using facebook::thrift::protocol::TBinaryProtocol;
using boost::shared_ptr;

int main() {
    shared_ptr<TMemoryBuffer> strBuffer(new TMemoryBuffer());
    shared_ptr<TBinaryProtocol> binaryProtcol(new TBinaryProtocol(strBuffer));

    Test a, b;
    //a.tt = (uint16_t)42;
    a.tt = 43;
    a.write(binaryProtcol.get());
    uint8_t *x;
    uint32_t sz;
    strBuffer->getBuffer(&x, &sz);
    printf("buffer len: %d\n",  sz);
    shared_ptr<TMemoryBuffer> strBuffer2(new TMemoryBuffer(x, sz));
    shared_ptr<TBinaryProtocol> binaryProtcol2(new TBinaryProtocol(strBuffer2));

    b.read(binaryProtcol2.get());
    printf("read from buffer %d\n", any_cast<int>(b.tt));
    assert(a == b);
    return 0;
}
