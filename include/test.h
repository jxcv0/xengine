#ifndef TEST_H_
#define TEST_H_

#include <stdio.h>
#include <time.h>

#define TEST_PASS 0
#define TEST_SKIP 77
#define TEST_FAIL 99

void assert_float_eq(const char *file, const char *func, int line,
                     float expected, float actual);
#define ASSERT_FLOAT_EQ(e, a) \
  assert_float_eq(__FILE__, __func__, __LINE__, e, a)

void test_end(void);

#endif  // TEST_H_
