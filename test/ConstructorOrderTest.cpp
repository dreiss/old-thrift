#include <iostream>
#include "gen-cpp/ConstructorOrderTest_types.h"

using namespace thrift::test;

int main() {
  OrderTest astruct(Type1(), 77, "abc", Type2());
  std::cout << "Passed Ordering test\n";
  return 0;
}
