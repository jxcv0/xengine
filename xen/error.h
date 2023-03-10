#ifndef ERROR_H_
#define ERROR_H_

#include <stdio.h>

#ifndef NDEBUG
#define ASSERT(_e, ...) if (!(_e)) { fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE); }
#else
#define ASSERT(_e, ...)
#endif

#endif // ERROR_H_
