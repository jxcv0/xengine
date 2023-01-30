#ifndef LIGHT_H_
#define LIGHT_H_

#include "lin.h"

#define LIGHT_RANGE_7 \
  { .m_constant = 1.0, .m_linear = 0.7, .m_quadratic = 1.8 }

#define LIGHT_RANGE_13 \
  { .m_constant = 1.0, .m_linear = 0.35, .m_quadratic = 0.44 }

#define LIGHT_RANGE_20 \
  { .m_constant = 1.0, .m_linear = 0.22, .m_quadratic = 0.2 }

#define LIGHT_RANGE_32 \
  { .m_constant = 1.0, .m_linear = 0.14, .m_quadratic = 0.07 }

#define LIGHT_RANGE_50 \
  { .m_constant = 1.0, .m_linear = 0.09, .m_quadratic = 0.32 }

#define LIGHT_RANGE_65 \
  { .m_constant = 1.0, .m_linear = 0.07, .m_quadratic = 0.017 }

#define LIGHT_RANGE_100 \
  { .m_constant = 1.0, .m_linear = 0.045, .m_quadratic = 0.0075 }

#define LIGHT_RANGE_160 \
  { .m_constant = 1.0, .m_linear = 0.027, .m_quadratic = 0.0028 }

#define LIGHT_RANGE_200 \
  { .m_constant = 1.0, .m_linear = 0.022, .m_quadratic = 0.0019 }

#define LIGHT_RANGE_325 \
  { .m_constant = 1.0, .m_linear = 0.014, .m_quadratic = 0.0007 }

#define LIGHT_RANGE_600 \
  { .m_constant = 1.0, .m_linear = 0.007, .m_quadratic = 0.0002 }

#define LIGHT_RANGE_3250 \
  { .m_constant = 1.0, .m_linear = 0.0014, .m_quadratic = 0.000007 }

#ifdef __cplusplus
extern "C" {
#endif

struct light {
  vec3 m_position;
  vec3 m_color;
  float m_constant;
  float m_linear;
  float m_quadratic;
};

#ifdef __cplusplus
}
#endif

#endif  // LIGHT_H_
