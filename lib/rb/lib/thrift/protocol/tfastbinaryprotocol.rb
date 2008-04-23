require File.join(File.dirname(__FILE__), '../../../ext/tfastbinaryprotocol')

=begin
The only change required for a transport to support TFastBinaryProtocol is to implement 3 methods:
  * string_buffer, which returns all unconsumed data as a string
  * refill_buffer(size), which refills the internal string buffer and
  returns the value of the string, returning atleast size bytes
  * consume!(size), which removes size bytes from the front of the buffer

See TMemoryBuffer and TBufferedTransport for examples.
=end