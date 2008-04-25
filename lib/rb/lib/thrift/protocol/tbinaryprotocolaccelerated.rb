require 'thrift/protocol/tbinaryprotocol'
require File.join(File.dirname(__FILE__), '../../../ext/tbinaryprotocolaccelerated')

=begin
The only change required for a transport to support TFastBinaryProtocol is to implement 2 methods:
  * borrow(size), which returns atleast _size_ bytes from the transport
  * consume!(size), which removes size bytes from the front of the buffer

See TMemoryBuffer and TBufferedTransport for examples.
=end

class TBinaryProtocolAcceleratedFactory < TProtocolFactory
  def getProtocol(trans)
    TBinaryProtocolAccelerated.new(trans)
  end
end
