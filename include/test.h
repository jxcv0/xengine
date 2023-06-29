#ifndef TEST_H_
#define TEST_H_

#include <stdio.h>
#include <time.h>

#define TEST_PASS 0
#define TEST_SKIP 77
#define TEST_FAIL 99

void assert_float_eq(const char* file, const char* func, int line, float exp,
                     float act);
#define ASSERT_FLOAT_EQ(e, a) \
  assert_float_eq(__FILE__, __func__, __LINE__, e, a)

void assert_true(const char* file, const char* func, int line, int val);
#define ASSERT_TRUE(v) assert_true(__FILE__, __func__, __LINE__, v)

void assert_false(const char* file, const char* func, int line, int val);
#define ASSERT_FALSE(v) assert_false(__FILE__, __func__, __LINE__, v)

void assert_int_eq(const char* file, const char* func, int line, int exp,
                   int act);
#define ASSERT_INT_EQ(e, a) assert_int_eq(__FILE__, __func__, __LINE__, e, a)

void test_start(int argc, char** argv);
void test_end(void);

#endif  // TEST_H_
