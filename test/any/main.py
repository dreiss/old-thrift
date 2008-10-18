import sys
sys.path.insert(0, '../../lib/py/build/lib.linux-i686-2.5/')
sys.path.insert(0, 'gen-py')
from thrift.protocol import TBinaryProtocol
from thrift.transport import TTransport
from anytest import ttypes

def zugzug(data):
  x = TBinaryProtocol.TBinaryProtocol(TTransport.TMemoryBuffer())
  data.write(x)
  x.trans._buffer.seek(0)
  result = ttypes.Test()
  result.read(x)
  return result

tst = ttypes.Test()
tst.tt = 103
rd = zugzug(tst)
print rd
assert tst == rd
n2 = ttypes.Test()
n2.tt = tst
rd = zugzug(n2)
print rd
assert n2 == rd
x = TBinaryProtocol.TBinaryProtocol(TTransport.TMemoryBuffer())
x.trans.write(open('test.dat', 'r').read())
x.trans._buffer.seek(0)
rd.read(x)
print rd
assert n2 == rd
