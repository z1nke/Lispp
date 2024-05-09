#include "lisp.h"

#include <gtest/gtest.h>

TEST(TestEval, Unit) {
  Environment env;
  const Object *unit = eval(read("()"), &env);
  ASSERT_EQ("()", unit->toString());
}
