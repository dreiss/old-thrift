# Case thrift rundown (in progress)
# Author: R.M.
# this file tests case handling in all major types/situations

# TODO:

namespace c CaseTest
namespace cpp casetest
namespace php CaseTest
namespace perl CaseTest
namespace java casetest
namespace rb CaseTest

# typedefs
typedef i32 typedeflowercase
typedef i32 typedef_lowercase_with_underscores
typedef i32 TypedefInitialCaps
typedef i32 Typedef_Initial_Caps_With_Underscores
typedef i32 TYPEDEFALLCAPS

# constants
const i32 constlowercase = 42
const i32 const_lowercase_with_underscores = 42
const i32 ConstInitialCaps = 42
const i32 Const_Initial_Caps_With_Underscores = 42
const i32 CONSTALLCAPS = 42

# enums
enum enumlowercase
{
    VALUE
}

enum enum_lowercase_with_underscores
{
    VALUE
}

enum EnumInitialCaps
{
    VALUE
}

enum Enum_Initial_Caps_With_Underscores
{
    VALUE
}

enum ENUMALLCAPS
{
    VALUE
}

# structs
struct structlowercase
{
    1: i32 value
}

struct struct_lowercase_with_underscores
{
    1: i32 value
}

struct StructInitialCaps
{
    1: i32 value
}

struct Struct_Initial_Caps_With_Underscores
{
    1: i32 value
}

struct STRUCTALLCAPS
{
    1: i32 value
}


# exceptions
exception exceptionlowercase
{
    1: string what
}

exception exception_lowercase_with_underscores
{
    1: string what
}

exception ExceptionInitialCaps
{
    1: string what
}

exception Exception_Initial_Caps_With_Underscores
{
    1: string what
}

exception EXCEPTIONALLCAPS
{
    1: string what
}

# service/methods
service servicelowercase
{
}
service service_lowercase_with_underscores
{
}
service ServiceInitialCaps
{
}
service Service_Initial_Caps_With_Underscores
{
}
service SERVICEALLCAPS
{
}

service CaseService
{
    void nothinglowercase(),
    void nothing_lowercase_with_underscores(),
    void NothingInitialCaps(),
    void Nothing_Initial_Caps_With_Underscores(),

    void somethinglowercase(i32 paramlowercase),
    void something_lowercase_with_underscores(i32 param_lowercase_with_underscores),
    void SomethingInitialCaps(i32 ParamInitalCaps),
    void Something_Initial_Caps_With_Underscores(i32 Param_Inital_Caps_With_Underscores)
}
