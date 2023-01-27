#ifndef LIGHT_H_
#define LIGHT_H_

#include "lin.h"

struct light {
  vec3 m_position;
  vec3 m_color;
  float m_constant;
  float m_linear;
  float m_quadratic;
};

#endif  // LIGHT_H_
