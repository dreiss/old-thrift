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

def bench_write(instance, factory, n=100000):
    start = time.time()
    n = 100000
    write_one(instance, factory, n)
    spent = time.time() - start
    return n/(spent * 1000.0)

def get_buffer(serialize):
    tran = TTransport.TMemoryBuffer()
    prot = TBinaryProtocol.TBinaryProtocol(tran)
    serialize.write(prot)
    return tran.getvalue()

def read_one(class_, buffer, factory, n):
    for _ in xrange(n):
        tran = TTransport.TMemoryBuffer(buffer)
        prot = factory.getProtocol(tran)

        class_().read(prot)

def bench_read(class_, instance, factory, n=100000):
    buffer = get_buffer(instance)
    start = time.time()
    n = 100000
    read_one(class_, buffer, factory, n)
    spent = time.time() - start
    return n/(spent * 1000.0)

def complex(class_, instance):
    print "write -> binary -> accelerated: %5.3f kHz" % \
        bench_write(instance, 
                TBinaryProtocol.TBinaryProtocolAcceleratedFactory())
    print "write -> binary -> python     : %5.3f kHz" % \
        bench_write(instance, 
                TBinaryProtocol.TBinaryProtocolFactory())
    print " read -> binary -> accelerated: %5.3f kHz" % \
        bench_read(class_, instance, 
                TBinaryProtocol.TBinaryProtocolAcceleratedFactory())
    print " read -> binary -> python     : %5.3f kHz" % \
        bench_read(class_, instance, 
                TBinaryProtocol.TBinaryProtocolFactory())

if __name__ == '__main__':
    instanity = Insanity(dict(userMap={1:2}, xtructs=[
        Xtruct(dict(string_thing="Zero", byte_thing=1, i32_thing=-3, i64_thing=5)),
        Xtruct(dict(string_thing="One", byte_thing=1, i32_thing=4, i64_thing=18))
      ]))
    print "instanity:"
    complex(Insanity, instanity)
    bonk = Bonk(dict(message="answer", type=42))
    print "bonk:"
    complex(Bonk, bonk)
