require 'thrift/protocol/tbinaryprotocol'
require 'tbinaryprotocolaccelerated'

=begin
The only change required for a transport to support TFastBinaryProtocol is to implement 2 methods:
  * borrow(size), which takes an optional argument and returns atleast _size_ bytes from the transport, 
                  or the default buffer size if no argument is given
  * consume!(size), which removes size bytes from the front of the buffer

See TMemoryBuffer and TBufferedTransport for examples.
=end

class TBinaryProtocolAcceleratedFactory < TProtocolFactory
  def getProtocol(trans)
    TBinaryProtocolAccelerated.new(trans)
  end
end
