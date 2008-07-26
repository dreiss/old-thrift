import sys, glob
import time

sys.path.insert(0, './gen-py')
sys.path.insert(0, glob.glob('../../lib/py/build/lib.*')[0])
from ThriftTest.ttypes import *

from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol

def bench(serialize, factory, n):
    for _ in xrange(n):
        otran = TTransport.TMemoryBuffer()
        oprot = factory.getProtocol(otran)

        serialize.write(oprot)

def bench_x(factory, n=100000):
    x = Xtruct(string_thing="Zero", byte_thing=1, i32_thing=-3, i64_thing=5)
    y = Xtruct(string_thing="One", byte_thing=1, i32_thing=4, i64_thing=18)
    ddd = Insanity(userMap={1:2}, xtructs=[x, y])
    start = time.time()
    n = 100000
    bench(ddd, factory, n)
    print "do %d in %0.2f sec" % (n, time.time() - start)

if __name__ == '__main__':
    print "encode: binary accelerated"
    bench_x(TBinaryProtocol.TBinaryProtocolAcceleratedFactory())
    print "encode: binary python"
    bench_x(TBinaryProtocol.TBinaryProtocolFactory())
