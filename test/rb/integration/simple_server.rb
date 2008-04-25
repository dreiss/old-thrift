$:.push File.dirname(__FILE__) + '/../gen-rb'
$:.push File.join(File.dirname(__FILE__), '../../../lib/rb/lib')

require 'thrift/transport/tsocket'
require 'thrift/protocol/tbinaryprotocol'
require 'thrift/server/tserver'
require 'ThriftTest'

class SimpleHandler
  [:testString, :testByte, :testI32, :testI64, :testDouble,
   :testStruct, :testMap, :testSet, :testList, :testNest,
   :testEnum, :testTypedef].each do |meth|

    define_method(meth) do |thing|
      thing
    end

  end

  def testInsanity(thing)
    num, uid = thing.userMap.find { true }
    return {uid => {num => thing}}
  end

  def testMapMap(thing)
    return {thing => {thing => thing}}
  end

  def testEnum(thing)
    return thing
  end

  def testTypedef(thing)
    return thing
  end

  def testException(thing)
    raise Thrift::Test::Xception, 'error'
  end
end

@handler   = SimpleHandler.new
@processor = Thrift::Test::ThriftTest::Processor.new(@handler)
@transport = TServerSocket.new(9090)
@server    = TThreadedServer.new(@processor, @transport)

@server.serve
