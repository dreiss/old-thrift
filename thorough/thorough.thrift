# Thorough thrift rundown (in progress)
# Author: R.M.
# this is a much more thorough version of StressTest


# TODO:
# - test out commenting stuff?
# - look at ConstantsDemo.thrift
# - optional & required keywords

namespace c Thorough
namespace cpp thorough
namespace php Thorough
namespace perl Thorough
namespace java thorough
namespace rb Thorough

# typedefs

typedef bool Bool
typedef byte Byte
typedef i16 I16
typedef i32 I32
typedef i64 I64
typedef double Double
typedef string String

# constants

const i32 CONST_I32 = 42
const string CONST_STRING = "hello world"

# enums 

/*
enum Auto
{
     CAT,
     DOG,
     COW,
     HORSE,
     ELEPHANT,
     HUMAN
}
*/

enum Complete
{
     CAT = 1,
     DOG = 2,
     COW = 3,
     HORSE = 4,
     ELEPHANT = 5,
     HUMAN = 6
}

/* thrift cpp doesn't support enums with identical values

enum Partial
{
     CAT = 1,
     DOG = 2,
     COW = 3,
     HORSE = 4,
     ELEPHANT = 5,
     HUMAN = 6
}

enum Skip
{
     CAT = 1,
     DOG = 2,
     COW = 3,
     HORSE = 7,
     ELEPHANT = 42,
     HUMAN = 44
}

enum All
{
     CAT = 1,
     DOG = 2,
     COW = 3,
     HORSE,
     ELEPHANT = 42,
     HUMAN
}
 */

# exceptions (complex below)

exception Simple
{
    1: string what
}

# structs

struct SimpleBool
{
    1: bool value
}

struct SimpleByte
{
    1: byte value
}

struct SimpleI16
{
    1: i16 value 
}

struct SimpleI32
{
    1: i32 value 
}

struct SimpleI64
{
    1: i64 value 
}

struct SimpleDouble
{
    1: double value 
}

struct SimpleString
{
    1: string value 
}

struct SimpleTypedef
{
    1: String value 
}

struct SimpleEnum
{
    1: Complete value
}

struct SimpleException
{
    1: Simple value 
}

struct SimpleAll
{
    1: bool            value_bool,
    2: byte            value_byte,
    3: i16             value_i16,
    4: i32             value_i32, 
    5: i64             value_i64, 
    6: double          value_double, 
    7: string          value_string,
    8: Double          value_typedef,
}

struct InitsBool
{
    1: bool value = 1
}

struct InitsByte
{
    1: byte value = 2
}

struct InitsI16
{
    1: i16 value = 3
}

struct InitsI32
{
    1: i32 value = 4
}

struct InitsI64
{
    1: i64 value = 5
}

struct InitsDouble
{
    1: double value = 6.6
}

struct InitsString
{
    1: string value = "string"
}

struct InitsTypedef
{
    1: String value = "string"
}

struct InitsEnum
{
    1: Complete value = ELEPHANT
}

struct InitsAll
{
    1: bool   value_bool = 1,
    2: byte   value_byte = 2,
    3: i16    value_i16 = 3,
    4: i32    value_i32 = 4, 
    5: i64    value_i64 = 5, 
    6: double value_double = 6.6, 
    7: string value_string = "string"
    8: String value_typedef = "string"
}

struct TypedefsAll
{
    1: Bool   value_bool,
    2: Byte   value_byte,
    3: I16    value_i16,
    4: I32    value_i32,
    5: I64    value_i64,
    6: Double value_double,
    7: String value_string,
}

struct NumberingAll
{
    2: bool      value_bool,
    3: byte      value_byte,
    10: i16      value_i16,
    42: i32      value_i32, 
    43: i64      value_i64, 
    44: double   value_double, 
    999: string  value_string,
    9999: Double value_typedef
}

struct HighMember
{
    42: i32 value
}

# exceptions (complex)

exception Complex
{
    1: i32       count,
    2: string    what,
    3: SimpleAll simple
}


# service

service Thorough
{
    # base types
    void void_void(),
    bool bool_bool(bool param),
    byte byte_byte(byte param),
    i16 i16_i16(i16 param),
    i32 i32_i32(i32 param),
    i64 i64_i64(i64 param),
    double double_double(double param),
    string string_string(string param),

    # numbered params
    void number_none(i32 one, i32 two),
    void number_complete(1:i32 one, 2:i32 two),
    void number_skip(1:i32 one, 4:i32 two),
    void number_partial(i32 one, 2:i32 two),
    void number_all(i32 one, 4:i32 two),

    # typedefs
#    Bool td_Bool_Bool(Bool param),
#    Byte td_Byte_Byte(Byte param),
#    I16 td_I16_I16(I16 param),
#    I32 td_I32_I32(I32 param),
#    I64 td_I64_I64(I64 param),
#    Double td_Double_Double(Double param),
#    String td_String_String(String param),

    # enums
    /* see enum comment above
    Auto Auto_Auto(Auto param),
    */
    Complete Complete_Complete(Complete param),
    /* see enum comment above
    Partial Partial_Partial(Partial param),
    Skip Skip_Skip(Skip param),
    All All_All(All param),
     */

    # structs
    SimpleBool SimpleBool_SimpleBool(SimpleBool param),
    SimpleByte SimpleByte_SimpleByte(SimpleByte param),
    SimpleI16 SimpleI16_SimpleI16(SimpleI16 param),
    SimpleI32 SimpleI32_SimpleI32(SimpleI32 param),
    SimpleI64 SimpleI64_SimpleI64(SimpleI64 param),
    SimpleDouble SimpleDouble_SimpleDouble(SimpleDouble param),
    SimpleString SimpleString_SimpleString(SimpleString param),
    SimpleTypedef SimpleTypedef_SimpleTypedef(SimpleTypedef param),
    SimpleEnum SimpleEnum_SimpleEnum(SimpleEnum param),
    SimpleAll SimpleAll_SimpleAll(SimpleAll param),
    /* thrift cpp can't handle this: Simple void_Simple(), */
    SimpleException void_SimpleException()

    NumberingAll NumberingAll_NumberingAll(NumberingAll param),

    HighMember HighMember_HighMember(HighMember param),

    # collections
    list<i32> list_i32_list_i32(list<i32> param)
    list<string> list_string_list_string(list<string> param)
    list<SimpleI32> list_SimpleI32_list_SimpleI32(list<SimpleI32> param)
    set<i32> set_i32_set_i32(set<i32> param)
    set<string> set_string_set_string(set<string> param)
    map<i32, i32> map_i32_map_i32(map<i32, i32> param)
    map<string, string> map_string_map_string(map<string, string> param)
    map<string, SimpleI32> map_string_SimpleI32_map_string_SimpleI32(map<string, SimpleI32> param)

    # exceptions
    void throws_simple() throws (Simple s),
    void throws_complex() throws (Complex c),
    void throws_multiple_simple() throws (Simple s, Complex c),
    void throws_multiple_complex() throws (Simple s, Complex c),
}
