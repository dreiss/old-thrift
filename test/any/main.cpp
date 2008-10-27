#include <iostream>
#include <climits>
#include <cassert>
#include <boost/shared_ptr.hpp>
#include <transport/TBufferTransports.h>
#include <transport/TFDTransport.h>
#include <protocol/TBinaryProtocol.h>
#include <TAny.h>
#include "anytest_types.h"

using facebook::thrift::any_cast;
using facebook::thrift::struct_cast;
using facebook::thrift::TAny;
using facebook::thrift::ThriftBase;
using facebook::thrift::transport::TMemoryBuffer;
using facebook::thrift::transport::TFDTransport;
using facebook::thrift::protocol::TBinaryProtocol;
using boost::shared_ptr;

Test *zugzug(Test &a)
{
    Test *b = new Test();
    shared_ptr<TMemoryBuffer> strBuffer(new TMemoryBuffer());
    shared_ptr<TBinaryProtocol> binaryProtcol(new TBinaryProtocol(strBuffer));

    a.write(binaryProtcol.get());
    uint8_t *x;
    uint32_t sz;
    strBuffer->getBuffer(&x, &sz);
    printf("buffer len: %d\n",  sz);
    shared_ptr<TMemoryBuffer> strBuffer2(new TMemoryBuffer(x, sz));
    shared_ptr<TBinaryProtocol> binaryProtcol2(new TBinaryProtocol(strBuffer2));

    b->read(binaryProtcol2.get());
    return b;
}
int main() {
    Test a, *b;
    //a.tt = (uint16_t)42;
    a.tt = 43;
    b = zugzug(a);
    assert(a == *b);
    printf("read from buffer %d\n", any_cast<int>(b->tt));
    delete b;
    a.tt = 41.14;
    b = zugzug(a);
    assert(a == *b);
    printf("read from buffer %0.2f\n", any_cast<double>(b->tt));
    delete b;

    a.tt = new Test();
    struct_cast<Test*>(a.tt)->tt = 5;
    b = zugzug(a);
    TAny x = struct_cast<Test*>(b->tt)->tt;
    printf("read from buffer %d\n", any_cast<int>(x));
    FILE *test_data = fopen("test.dat", "r");
    shared_ptr<TFDTransport> ft(new TFDTransport(fileno(test_data)));
    shared_ptr<TBinaryProtocol> bp(new TBinaryProtocol(ft));
    b->read(bp.get());
    x = struct_cast<Test*>(b->tt)->tt;
    assert(any_cast<int>(x) == 103);
    return 0;
}
