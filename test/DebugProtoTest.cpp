#include <iostream>
#include <cmath>
#include "gen-cpp/DebugProtoTest_types.h"
#include "../lib/cpp/src/protocol/TDebugProtocol.h"

int main() {
  using std::cout;
  using std::endl;
  using namespace thrift::test::debug;


  OneOfEach ooe(true, false, 0xd6, 27000, 1<<24, (int64_t)6000 * 1000 * 1000,
		M_PI, "Debug THIS!", "\xd7\n\a\t", true, 
		std::string("\x00\x41\xFF\x7F",4));

  // was:
//  ooe.im_true   = true;
//  ooe.im_false  = false;
//  ooe.a_bite    = 0xd6;
//  ooe.integer16 = 27000;
//  ooe.integer32 = 1<<24;
//  ooe.integer64 = (uint64_t)6000 * 1000 * 1000;
//  ooe.double_precision = M_PI;
//  ooe.some_characters  = "Debug THIS!";
//  ooe.zomg_unicode     = "\xd7\n\a\t";

  cout << facebook::thrift::ThriftDebugString(ooe) << endl << endl;

  std::string expected_debug_string = 
    "OneOfEach {\n"
    "  01: im_true (bool) = true,\n"
    "  02: im_false (bool) = false,\n"
    "  03: a_bite (byte) = 0xd6,\n"
    "  04: integer16 (i16) = 27000,\n"
    "  05: integer32 (i32) = 16777216,\n"
    "  06: integer64 (i64) = 6000000000,\n"
    "  07: double_precision (double) = 3.141592653589793,\n"
    "  08: some_characters (string) = \"Debug THIS!\",\n"
    "  09: zomg_unicode (string) = \"\\xd7\\n\\a\\t\",\n"
    "  10: what_who (bool) = true,\n"
    "  11: base64 (string) = \"\\x00A\\xff\\x7f\",\n"
    "}";
  assert(expected_debug_string == facebook::thrift::ThriftDebugString(ooe));

  Nesting n;
  n.my_ooe = ooe;
  n.my_ooe.integer16 = 16;
  n.my_ooe.integer32 = 32;
  n.my_ooe.integer64 = 64;
  n.my_ooe.double_precision = (std::sqrt(5)+1)/2;
  n.my_ooe.some_characters  = ":R (me going \"rrrr\")";
  n.my_ooe.zomg_unicode     = "\xd3\x80\xe2\x85\xae\xce\x9d\x20"
                              "\xd0\x9d\xce\xbf\xe2\x85\xbf\xd0\xbe\xc9\xa1\xd0\xb3\xd0\xb0\xcf\x81\xe2\x84\x8e"
                              "\x20\xce\x91\x74\x74\xce\xb1\xe2\x85\xbd\xce\xba\xc7\x83\xe2\x80\xbc";
  n.my_bonk.type    = 31337;
  n.my_bonk.message = "I am a bonk... xor!";

  cout << facebook::thrift::ThriftDebugString(n) << endl << endl;


  HolyMoley hm;

  hm.big.push_back(ooe);
  hm.big.push_back(n.my_ooe);
  hm.big[0].a_bite = 0x22;
  hm.big[1].a_bite = 0x33;

  std::vector<std::string> stage1;
  stage1.push_back("and a one");
  stage1.push_back("and a two");
  hm.contain.insert(stage1);
  stage1.clear();
  stage1.push_back("then a one, two");
  stage1.push_back("three!");
  stage1.push_back("FOUR!!");
  hm.contain.insert(stage1);
  stage1.clear();
  hm.contain.insert(stage1);

  std::vector<Bonk> stage2;
  hm.bonks["nothing"] = stage2;
  stage2.resize(stage2.size()+1);
  stage2.back().type = 1;
  stage2.back().message = "Wait.";
  stage2.resize(stage2.size()+1);
  stage2.back().type = 2;
  stage2.back().message = "What?";
  hm.bonks["something"] = stage2;
  stage2.clear();
  stage2.resize(stage2.size()+1);
  stage2.back().type = 3;
  stage2.back().message = "quoth";
  stage2.resize(stage2.size()+1);
  stage2.back().type = 4;
  stage2.back().message = "the raven";
  stage2.resize(stage2.size()+1);
  stage2.back().type = 5;
  stage2.back().message = "nevermore";
  hm.bonks["poe"] = stage2;

  cout << facebook::thrift::ThriftDebugString(hm) << endl << endl;


  return 0;
}
