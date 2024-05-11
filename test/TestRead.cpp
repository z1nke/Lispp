#include "Common.h"
#include "lisp.h"

void testUnit() {
  const Object *unit = read("()");
  UNIT_TEST("()", unit->toString());

  unit = read("();xxx");
  UNIT_TEST("()", unit->toString());
}

int main() {
  testUnit();
  std::cout << testPass << "/" << testCount << " (passed "
            << testPass * 100.0 / testCount << "%)" << std::endl;
  return mainRet;
}
