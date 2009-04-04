namespace cpp thrift.test

struct Type1 {
}

struct Type2 {
}

struct OrderTest {
  5: required i32 field_5;
  required string field_minus1;
  1: required Type1 field_1;
  required Type2 field_minus2;
}
