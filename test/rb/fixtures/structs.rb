require 'thrift/thrift'

module Fixtures
  module Structs
    # struct OneOfEach {
    #   1: bool im_true,
    #   2: bool im_false,
    #   3: byte a_bite,
    #   4: i16 integer16,
    #   5: i32 integer32,
    #   6: i64 integer64,
    #   7: double double_precision,
    #   8: string some_characters,
    #   9: string zomg_unicode,
    #   10: bool what_who,
    #   11: binary base64,
    # }
    class OneOfEach
      include ThriftStruct
      attr_accessor :im_true, :im_false, :a_bite, :integer16, :integer32, :integer64, :double_precision, :some_characters, :zomg_unicode, :what_who, :base64
      FIELDS = {
        1 => {:type => TType::BOOL, :name => 'im_true'},
        2 => {:type => TType::BOOL, :name => 'im_false'},
        3 => {:type => TType::BYTE, :name => 'a_bite'},
        4 => {:type => TType::I16, :name => 'integer16'},
        5 => {:type => TType::I32, :name => 'integer32'},
        6 => {:type => TType::I64, :name => 'integer64'},
        7 => {:type => TType::DOUBLE, :name => 'double_precision'},
        8 => {:type => TType::STRING, :name => 'some_characters'},
        9 => {:type => TType::STRING, :name => 'zomg_unicode'},
        10 => {:type => TType::BOOL, :name => 'what_who'},
        11 => {:type => TType::STRING, :name => 'base64'}
      }

      # Added for assert_equal
      def ==(other)
        [:im_true, :im_false, :a_bite, :integer16, :integer32, :integer64, :double_precision, :some_characters, :zomg_unicode, :what_who, :base64].each do |f|
          var = "@#{f}"
          return false if instance_variable_get(var) != other.instance_variable_get(var)
        end
        true
      end
    end

    # struct Nested1 {
    #   1: list<OneOfEach> a_list
    #   2: map<i32, OneOfEach> i32_map
    #   3: map<i64, OneOfEach> i64_map
    #   4: map<double, OneOfEach> dbl_map
    #   5: map<string, OneOfEach> str_map
    # }
    class Nested1
      include ThriftStruct
      attr_accessor :a_list, :i32_map, :i64_map, :dbl_map, :str_map
      FIELDS = {
        1 => {:type => TType::LIST, :name => 'a_list', :element => {:type => TType::STRUCT, :class => OneOfEach}},
        2 => {:type => TType::MAP, :name => 'i32_map', :key => {:type => TType::I32}, :value => {:type => TType::STRUCT, :class => OneOfEach}},
        3 => {:type => TType::MAP, :name => 'i64_map', :key => {:type => TType::I64}, :value => {:type => TType::STRUCT, :class => OneOfEach}},
        4 => {:type => TType::MAP, :name => 'dbl_map', :key => {:type => TType::DOUBLE}, :value => {:type => TType::STRUCT, :class => OneOfEach}},
        5 => {:type => TType::MAP, :name => 'str_map', :key => {:type => TType::STRING}, :value => {:type => TType::STRUCT, :class => OneOfEach}}
      }
    end

    # struct Nested2 {
    #   1: list<Nested1> a_list
    #   2: map<i32, Nested1> i32_map
    #   3: map<i64, Nested1> i64_map
    #   4: map<double, Nested1> dbl_map
    #   5: map<string, Nested1> str_map
    # }
    class Nested2
      include ThriftStruct
      attr_accessor :a_list, :i32_map, :i64_map, :dbl_map, :str_map
      FIELDS = {
        1 => {:type => TType::LIST, :name => 'a_list', :element => {:type => TType::STRUCT, :class => Nested1}},
        2 => {:type => TType::MAP, :name => 'i32_map', :key => {:type => TType::I32}, :value => {:type => TType::STRUCT, :class => Nested1}},
        3 => {:type => TType::MAP, :name => 'i64_map', :key => {:type => TType::I64}, :value => {:type => TType::STRUCT, :class => Nested1}},
        4 => {:type => TType::MAP, :name => 'dbl_map', :key => {:type => TType::DOUBLE}, :value => {:type => TType::STRUCT, :class => Nested1}},
        5 => {:type => TType::MAP, :name => 'str_map', :key => {:type => TType::STRING}, :value => {:type => TType::STRUCT, :class => Nested1}}
      }
    end

    # struct Nested3 {
    #   1: list<Nested2> a_list
    #   2: map<i32, Nested2> i32_map
    #   3: map<i64, Nested2> i64_map
    #   4: map<double, Nested2> dbl_map
    #   5: map<string, Nested2> str_map
    # }
    class Nested3
      include ThriftStruct
      attr_accessor :a_list, :i32_map, :i64_map, :dbl_map, :str_map
      FIELDS = {
        1 => {:type => TType::LIST, :name => 'a_list', :element => {:type => TType::STRUCT, :class => Nested2}},
        2 => {:type => TType::MAP, :name => 'i32_map', :key => {:type => TType::I32}, :value => {:type => TType::STRUCT, :class => Nested2}},
        3 => {:type => TType::MAP, :name => 'i64_map', :key => {:type => TType::I64}, :value => {:type => TType::STRUCT, :class => Nested2}},
        4 => {:type => TType::MAP, :name => 'dbl_map', :key => {:type => TType::DOUBLE}, :value => {:type => TType::STRUCT, :class => Nested2}},
        5 => {:type => TType::MAP, :name => 'str_map', :key => {:type => TType::STRING}, :value => {:type => TType::STRUCT, :class => Nested2}}
      }
    end

    # struct Nested4 {
    #   1: list<Nested3> a_list
    #   2: map<i32, Nested3> i32_map
    #   3: map<i64, Nested3> i64_map
    #   4: map<double, Nested3> dbl_map
    #   5: map<string, Nested3> str_map
    # }
    class Nested4
      include ThriftStruct
      attr_accessor :a_list, :i32_map, :i64_map, :dbl_map, :str_map
      FIELDS = {
        1 => {:type => TType::LIST, :name => 'a_list', :element => {:type => TType::STRUCT, :class => Nested3}},
        2 => {:type => TType::MAP, :name => 'i32_map', :key => {:type => TType::I32}, :value => {:type => TType::STRUCT, :class => Nested3}},
        3 => {:type => TType::MAP, :name => 'i64_map', :key => {:type => TType::I64}, :value => {:type => TType::STRUCT, :class => Nested3}},
        4 => {:type => TType::MAP, :name => 'dbl_map', :key => {:type => TType::DOUBLE}, :value => {:type => TType::STRUCT, :class => Nested3}},
        5 => {:type => TType::MAP, :name => 'str_map', :key => {:type => TType::STRING}, :value => {:type => TType::STRUCT, :class => Nested3}}
      }
    end
  end
end
