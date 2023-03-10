#ifndef ERROR_H_
#define ERROR_H_

#include <stdio.h>
#include <stdlib.h>

#ifndef NDEBUG
#define ASSERT(_e, ...)           \
  if (!(_e)) {                    \
    fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__); \
    exit(EXIT_FAILURE);           \
  }
#else
#define ASSERT(_e, ...)
#endif

#endif  // ERROR_H_
