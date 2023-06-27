#ifndef TEST_H_
#define TEST_H_

#include <stdio.h>
#include <time.h>

#define TEST_PASS 0
#define TEST_SKIP 77
#define TEST_FAIL 99

#define ASSERT_FLOAT_EQ(f1, f2) assert(fabs(f1 - f2) < (FLT_EPSILON * 10))

void test_pass(const char *file, const char *msg, ...) {}

void print_time(clock_t start, const char *msg) {
  printf("TIME: %s - %f s\n", msg, (double)(clock() - start) / CLOCKS_PER_SEC);
}

#endif  // TEST_H_
