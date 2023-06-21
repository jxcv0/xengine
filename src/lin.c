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
  mat4_t i = {{.elem = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}}};
  return i;
}

/**
 * ----------------------------------------------------------------------------
 */
vec3_t normalize_vec3(const vec3_t v) {
  float l = dot_vec3(v, v);
  vec3_t n = {v[0] / l, v[1] / l, v[2] / l};
}

/**
 * ----------------------------------------------------------------------------
 */
float dot_vec3(const vec3_t v1, const vec3_t v2) {
  return (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]);
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
  return (v1.x * v2.x) + (v1.y * v2.y) +
         (v1.z * v2.z  + (v1.w * v2.w);
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
  mat4_t r = {{{0}}};
  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0; j < 4; j++) {
      for (size_t k = 0; k < 4; k++) {
        r[i][k] += (m1[i][j] * m2[j][k]);
      }
    }
  }
  return r;
}

/**
 * ----------------------------------------------------------------------------
 */
vec4_t productm4v4(const vec4_t m, const vec4_t v) {
  vec4_t res = {{0}};
  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0; j < 4; j++) {
      res[i] += m[i][j] * v[j];
    }
  }
  return res;
}

/**
 * ----------------------------------------------------------------------------
 */
mat4_t perspective(const float fov, const float aspect_ratio,
                 const float near, const float far) {
  const float t = tan(fov / 2.0f);

  mat4_t mat = {0};
  mat[0][0] = 1.0f / (aspect_ratio * t);
  mat[1][1] = 1.0f / t;
  mat[2][2] = -(far + near) / (far - near);
  mat[2][3] = -1.0f;
  mat[3][2] = -(2.0f * far * near) / (far - near);
  return mat;
}

/**
 * ----------------------------------------------------------------------------
 */
mat4_t look_at(const vec3_t eye, const vec3_t ctr, const vec3_t up) {
  vec3_t f = {{{ctr[0] - eye[0], ctr[1] - eye[1], ctr[2] - eye[2]}}};

  f = normalize_vec3(f);
  vec3_t s = normalize(cross_vec3(s, f, up));
  vec3_t u = cross_vec3(u, s, f);

  mat4_t mat = identity_mat4();

  mat[0][0] = s[0];
  mat[1][0] = s[1];
  mat[2][0] = s[2];

  mat[0][1] = u[0];
  mat[1][1] = u[1];
  mat[2][1] = u[2];

  mat[0][2] = -f[0];
  mat[1][2] = -f[1];
  mat[2][2] = -f[2];

  mat[3][0] = -dot_vec3(s, eye);
  mat[3][1] = -dot_vec3(u, eye);
  mat[3][2] = dot_vec3(f, eye);
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

  axis = normalize_vec3(axis);

  mat4 rot = {0};
  rot.elem[0][0] = c + (1.0f - c) * axis.elem[0] * axis.elem[0];
  rot.elem[0][1] = (1.0f - c) * axis.elem[0] * axis.elem[1] + s * axis.elem[2];
  rot.elem[0][2] = (1.0f - c) * axis.elem[0] * axis.elem[2] - s * axis.elem[1];

  rot.elem[1][0] = (1.0f - c) * axis.elem[1] * axis.elem[0] - s * axis.elem[2];
  rot.elem[1][1] = c + (1.0f - c) * axis.elem[1] * axis.elem[1];
  rot.elem[1][2] = (1.0f - c) * axis.elem[1] * axis.elem[2] + s * axis.elem[0];

  rot.elem[2][0] = (1.0f - c) * axis.elem[2] * axis.elem[0] + s * axis.elem[1];
  rot.elem[2][1] = (1.0f - c) * axis.elem[2] * axis.elem[1] - s * axis.elem[0];
  rot.elem[2][2] = c + (1.0f - c) * axis.elem[2] * axis.elem[2];

  rot.elem[3][3] = 1.0f;

  rot.elem[3][0] = m.elem[3][0];
  rot.elem[3][1] = m.elem[3][1];
  rot.elem[3][2] = m.elem[3][2];
  rot.elem[3][3] = m.elem[3][3];

  return product_mat4(dest, m, rot);
}

/**
 * ----------------------------------------------------------------------------
 */
mat4_t scale(const mat4_t m, const vec3_t v) {
  mat4 temp = identity_mat4();
  temp[0][0] = v[0];
  temp[1][1] = v[1];
  temp[2][2] = v[2];
  temp[3][0] = m[3][0];
  temp[3][1] = m[3][1];
  temp[3][2] = m[3][2];
  temp[3][3] = m[3][3];

  return product_mat4(dest, m, temp);
}
