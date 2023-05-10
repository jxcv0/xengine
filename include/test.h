#ifndef TEST_H_
#define TEST_H_

#include <stdio.h>

#define TEST_BEGIN() printf("%s: Running tests.\n", __FILE__)
#define TEST_END() printf("%s: Tests passed.\n", __FILE__)
#define TEST() printf("\t- %s\n", __func__)

#define ASSERT_FLOAT_EQ(f1, f2) assert(fabs(f1 - f2) < (FLT_EPSILON * 10))

#endif  // TEST_H_
