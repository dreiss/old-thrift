import sys, glob
import time

sys.path.insert(0, './gen-py')
sys.path.insert(0, glob.glob('../../lib/py/build/lib.*')[0])
from ThriftTest.ttypes import *

from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

def write_one(serialize, factory, n):
    for _ in xrange(n):
        otran = TTransport.TMemoryBuffer()
        oprot = factory.getProtocol(otran)

        serialize.write(oprot)

def bench_write(factory, n=100000):
    x = Xtruct(string_thing="Zero", byte_thing=1, i32_thing=-3, i64_thing=5)
    y = Xtruct(string_thing="One", byte_thing=1, i32_thing=4, i64_thing=18)
    ddd = Insanity(userMap={1:2}, xtructs=[x, y])
    start = time.time()
    n = 100000
    write_one(ddd, factory, n)
    spent = time.time() - start
    return n/(spent * 1000.0)

def get_buffer():
    x = Xtruct(string_thing="Zero", byte_thing=1, i32_thing=-3, i64_thing=5)
    y = Xtruct(string_thing="One", byte_thing=1, i32_thing=4, i64_thing=18)
    serialize = Insanity(userMap={1:2}, xtructs=[x, y])
    tran = TTransport.TMemoryBuffer()
    prot = TBinaryProtocol.TBinaryProtocol(tran)
    serialize.write(prot)
    return tran.getvalue()

def read_one(class_, buffer, factory, n):
    for _ in xrange(n):
        tran = TTransport.TMemoryBuffer(buffer)
        prot = factory.getProtocol(tran)

        class_().read(prot)

def bench_read(factory, n=100000):
    buffer = get_buffer()
    start = time.time()
    n = 100000
    read_one(Insanity, buffer, factory, n)
    spent = time.time() - start
    return n/(spent * 1000.0)

if __name__ == '__main__':
    print "write -> binary -> accelerated: %5.3f kHz" % \
        bench_write(TBinaryProtocol.TBinaryProtocolAcceleratedFactory())
    print "write -> binary -> python     : %5.3f kHz" % \
        bench_write(TBinaryProtocol.TBinaryProtocolFactory())
    print " read -> binary -> accelerated: %5.3f kHz" % \
        bench_read(TBinaryProtocol.TBinaryProtocolAcceleratedFactory())
    print " read -> binary -> python     : %5.3f kHz" % \
        bench_write(TBinaryProtocol.TBinaryProtocolFactory())
