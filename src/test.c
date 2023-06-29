#include "test.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define NONE "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

/**
 * ----------------------------------------------------------------------------
 */
void assert_float_eq(const char* file, const char* func, int line,
                     float expected, float actual) {
  if (fabs(expected - actual) > (FLT_EPSILON * 10)) {
    printf(RED "[FAILURE]: %s:%d %s\n" NONE "\tExpected: %f\n\tActual: %f\n",
           file, line, func, expected, actual);
    exit(EXIT_SUCCESS);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void assert_true(const char* file, const char* func, int line, int val) {
  if (!val) {
    printf(RED "[FAILURE]: %s:%d %s\n " NONE "\tExpected true value is false\n",
           file, line, func);
    exit(EXIT_SUCCESS);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void assert_false(const char* file, const char* func, int line, int val) {
  if (val) {
    printf(RED "[FAILURE]: %s:%d %s\n" NONE "\tExpected false value is true\n",
           file, line, func);
    exit(EXIT_SUCCESS);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void assert_int_eq(const char* file, const char* func, int line, int exp,
                   int act) {
  if (exp != act) {
    printf(RED "[FAILURE]: %s:%d %s\n" NONE "\tExpected: %d\n\tActual: %d\n",
           file, line, func, exp, act);
    exit(EXIT_SUCCESS);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void test_end(const char* file) {
  printf(GREEN "[TEST PASSED] " NONE "%s\n", file);
  exit(EXIT_SUCCESS);
}
