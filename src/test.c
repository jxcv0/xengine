#include "test.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * ----------------------------------------------------------------------------
 */
void assert_float_eq(const char *file, const char *func, int line,
                     float expected, float actual) {
  if (fabs(expected - actual) > (FLT_EPSILON * 10)) {
    printf("FAILURE: %s:%d %s\n\tExpected: %f\n\tActual: %f\n", file, line,
           func, expected, actual);
    exit(EXIT_SUCCESS);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void test_end(void) {
  printf("ALL TESTS PASSED\n");
  exit(EXIT_SUCCESS);
}