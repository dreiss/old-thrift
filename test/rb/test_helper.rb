$:.push File.dirname(__FILE__) + '/gen-rb'
$:.push File.join(File.dirname(__FILE__), '../../lib/rb/lib')

require 'test/unit'

module ThriftStruct
  def ==(other)
    return false unless other.is_a? self.class
    self.class.const_get(:FIELDS).collect {|fid, data| data[:name] }.all? do |field|
      send(field) == other.send(field)
    end
  end
end