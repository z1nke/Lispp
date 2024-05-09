#include "lisp.h"

#include <gtest/gtest.h>

TEST(TestRead, Unit) {
  const Object *unit = read("()");
  ASSERT_EQ("()", unit->toString());
}