#ifndef LIGHT_H_
#define LIGHT_H_

#include "vec3.h"

struct Light {
  Vec3 m_pos;
  Vec3 m_col;
  float m_const;
  float m_lin;
  float m_quad;
};

#endif  // LIGHT_H_
