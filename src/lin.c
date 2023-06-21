#include "lin.h"

#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

/**
 * ----------------------------------------------------------------------------
 */
void print_mat4(const mat4_t m) {
  printf("{ ");
  for (int i = 0; i < 4; i++) {
    printf("{ %f %f %f %f } ", m.elem[i][0], m.elem[i][1], m.elem[i][2],
           m.elem[i][3]);
  }
  printf("}\n");
}

/**
 * ----------------------------------------------------------------------------
 */
void print_vec(const float *v, size_t n) {
  printf("{ ");
  for (size_t i = 0; i < n; i++) {
    printf("%f ", v[i]);
  }
  printf("}\n");
}

/**
 * ----------------------------------------------------------------------------
 */
float radians(const float degrees) { return (degrees * M_PI) / 180.0f; }

/**
 * ----------------------------------------------------------------------------
 */
mat4_t identity_mat4(void) {
  mat4_t i = {.elem = {{1.0f, 0.0f, 0.0f, 0.0f},
                       {0.0f, 1.0f, 0.0f, 0.0f},
                       {0.0f, 0.0f, 1.0f, 0.0f},
                       {0.0f, 0.0f, 0.0f, 1.0f}}};
  return i;
}

/**
 * ----------------------------------------------------------------------------
 */
vec3_t normalize_vec3(const vec3_t v) {
  float l = sqrt(dot_vec3(v, v));
  vec3_t n = {.elem = {v.elem[0] / l, v.elem[1] / l, v.elem[2] / l}};
  return n;
}

/**
 * ----------------------------------------------------------------------------
 */
float dot(const float *v1, const float *v2, size_t n) {
  float res = 0.0f;
  for (size_t i = 0; i < n; i++) {
    res += v1[i] * v2[i];
  }
  return res;
}

/**
 * ----------------------------------------------------------------------------
 */
float dot_vec3(const vec3_t v1, const vec3_t v2) {
  return dot(v1.elem, v2.elem, 3);
}

/**
 * ----------------------------------------------------------------------------
 */
int cmp_vec3(const vec3_t v1, const vec3_t v2) {
  for (int i = 0; i < 3; i++) {
    if (fabs(v1.elem[i] - v2.elem[i]) > FLT_EPSILON) {
      return 0;
    }
  }
  return 1;
}

/**
 * ----------------------------------------------------------------------------
 */
int cmp_vec2(const vec2_t v1, const vec2_t v2) {
  for (int i = 0; i < 2; i++) {
    if (fabs(v1.elem[i] - v2.elem[i]) > FLT_EPSILON) {
      return 0;
    }
  }
  return 1;
}

/**
 * ----------------------------------------------------------------------------
 */
float dot_vec4(const vec4_t v1, const vec4_t v2) {
  return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w);
}
/**
 * ----------------------------------------------------------------------------
 */
vec3_t cross_vec3(const struct vec3 v1, const struct vec3 v2) {
  struct vec3 r = {
      {{(v1.y * v2.z) - (v1.z * v2.y), (v1.z * v2.x) - (v1.x * v2.z),
        (v1.x * v2.y) - (v1.y * v2.x)}}};
  return r;
}

/**
 * ----------------------------------------------------------------------------
 */
mat4_t product_mat4(const mat4_t m1, const mat4_t m2) {
  mat4_t r = {0};
  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0; j < 4; j++) {
      for (size_t k = 0; k < 4; k++) {
        r.elem[i][k] += m1.elem[i][j] * m2.elem[j][k];
      }
    }
  }
  return r;
}

/**
 * ----------------------------------------------------------------------------
 */
vec4_t productm4v4(const mat4_t m, const vec4_t v) {
  vec4_t res = {0};
  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0; j < 4; j++) {
      res.elem[i] += m.elem[i][j] * v.elem[j];
    }
  }
  return res;
}

/**
 * ----------------------------------------------------------------------------
 */
mat4_t perspective(const float fov, const float aspect_ratio, const float near,
                   const float far) {
  const float t = tan(fov / 2.0f);

  mat4_t mat = {0};
  mat.elem[0][0] = 1.0f / (aspect_ratio * t);
  mat.elem[1][1] = 1.0f / t;
  mat.elem[2][2] = -(far + near) / (far - near);
  mat.elem[2][3] = -1.0f;
  mat.elem[3][2] = -(2.0f * far * near) / (far - near);
  return mat;
}

/**
 * ----------------------------------------------------------------------------
 */
mat4_t look_at(const vec3_t eye, const vec3_t ctr, const vec3_t up) {
  vec3_t f = {{{ctr.elem[0] - eye.elem[0], ctr.elem[1] - eye.elem[1],
                ctr.elem[2] - eye.elem[2]}}};

  f = normalize_vec3(f);
  vec3_t s = normalize_vec3(cross_vec3(f, up));
  vec3_t u = cross_vec3(s, f);

  mat4_t mat = identity_mat4();

  mat.elem[0][0] = s.elem[0];
  mat.elem[1][0] = s.elem[1];
  mat.elem[2][0] = s.elem[2];

  mat.elem[0][1] = u.elem[0];
  mat.elem[1][1] = u.elem[1];
  mat.elem[2][1] = u.elem[2];

  mat.elem[0][2] = -f.elem[0];
  mat.elem[1][2] = -f.elem[1];
  mat.elem[2][2] = -f.elem[2];

  mat.elem[3][0] = -dot_vec3(s, eye);
  mat.elem[3][1] = -dot_vec3(u, eye);
  mat.elem[3][2] = dot_vec3(f, eye);
  return mat;
}

/**
 * ----------------------------------------------------------------------------
 */
mat4_t translate(const mat4_t m, const vec3_t v) {
  mat4_t res = m;
  res.elem[3][0] += v.elem[0];
  res.elem[3][1] += v.elem[1];
  res.elem[3][2] += v.elem[2];
  return res;
}

/**
 * ----------------------------------------------------------------------------
 */
mat4_t rotate(const mat4_t m, const vec3_t axis, const float rads) {
  const float c = cos(rads);
  const float s = sin(rads);

  vec3_t a = normalize_vec3(axis);

  mat4_t rot = {0};
  rot.elem[0][0] = c + (1.0f - c) * a.elem[0] * a.elem[0];
  rot.elem[0][1] = (1.0f - c) * a.elem[0] * a.elem[1] + s * a.elem[2];
  rot.elem[0][2] = (1.0f - c) * a.elem[0] * a.elem[2] - s * a.elem[1];

  rot.elem[1][0] = (1.0f - c) * a.elem[1] * a.elem[0] - s * a.elem[2];
  rot.elem[1][1] = c + (1.0f - c) * a.elem[1] * a.elem[1];
  rot.elem[1][2] = (1.0f - c) * a.elem[1] * a.elem[2] + s * a.elem[0];

  rot.elem[2][0] = (1.0f - c) * a.elem[2] * a.elem[0] + s * a.elem[1];
  rot.elem[2][1] = (1.0f - c) * a.elem[2] * a.elem[1] - s * a.elem[0];
  rot.elem[2][2] = c + (1.0f - c) * a.elem[2] * a.elem[2];

  rot.elem[3][3] = 1.0f;

  // rot.elem[3][0] = m.elem[3][0];
  // rot.elem[3][1] = m.elem[3][1];
  // rot.elem[3][2] = m.elem[3][2];
  // rot.elem[3][3] = m.elem[3][3];

  return product_mat4(m, rot);
}

/**
 * ----------------------------------------------------------------------------
 */
mat4_t scale(const mat4_t m, const vec3_t v) {
  mat4_t sm = identity_mat4();
  sm.elem[0][0] = v.elem[0];
  sm.elem[1][1] = v.elem[1];
  sm.elem[2][2] = v.elem[2];
  sm.elem[3][0] = m.elem[3][0];
  sm.elem[3][1] = m.elem[3][1];
  sm.elem[3][2] = m.elem[3][2];
  sm.elem[3][3] = m.elem[3][3];

  return product_mat4(m, sm);
}
