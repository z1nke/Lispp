#ifndef LISPP_TEST_COMMON_H
#define LISPP_TEST_COMMON_H

static int mainRet = 0;
static int testCount = 0;
static int testPass = 0;

#define UNIT_TEST_BASE(EQUALITY, EXPECT, ACTUAL)                               \
  do {                                                                         \
    testCount++;                                                               \
    if (EQUALITY)                                                              \
      testPass++;                                                              \
    else {                                                                     \
      std::cerr << __FILE__ << ":" << __LINE__ << ":  expect: " << EXPECT      \
                << " actual: " << ACTUAL << std::endl;                         \
      mainRet = 1;                                                             \
    }                                                                          \
  } while (0)

#define UNIT_TEST(EXPECT, ACTUAL)                                              \
  UNIT_TEST_BASE((EXPECT) == (ACTUAL), (EXPECT), (ACTUAL))

#endif