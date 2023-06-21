#ifndef LIGHT_H_
#define LIGHT_H_

#include "lin.h"

#define LIGHT_RANGE_7 \
  { .constant = 1.0, .linear = 0.7, .quadratic = 1.8 }

#define LIGHT_RANGE_13 \
  { .constant = 1.0, .linear = 0.35, .quadratic = 0.44 }

#define LIGHT_RANGE_20 \
  { .constant = 1.0, .linear = 0.22, .quadratic = 0.2 }

#define LIGHT_RANGE_32 \
  { .constant = 1.0, .linear = 0.14, .quadratic = 0.07 }

#define LIGHT_RANGE_50 \
  { .constant = 1.0, .linear = 0.09, .quadratic = 0.32 }

#define LIGHT_RANGE_65 \
  { .constant = 1.0, .linear = 0.07, .quadratic = 0.017 }

#define LIGHT_RANGE_100 \
  { .constant = 1.0, .linear = 0.045, .quadratic = 0.0075 }

#define LIGHT_RANGE_160 \
  { .constant = 1.0, .linear = 0.027, .quadratic = 0.0028 }

#define LIGHT_RANGE_200 \
  { .constant = 1.0, .linear = 0.022, .quadratic = 0.0019 }

#define LIGHT_RANGE_325 \
  { .constant = 1.0, .linear = 0.014, .quadratic = 0.0007 }

#define LIGHT_RANGE_600 \
  { .constant = 1.0, .linear = 0.007, .quadratic = 0.0002 }

#define LIGHT_RANGE_3250 \
  { .constant = 1.0, .linear = 0.0014, .quadratic = 0.000007 }

struct light {
  vec3_t position;
  vec3_t color;
  float constant;
  float linear;
  float quadratic;
};

#endif  // LIGHT_H_
