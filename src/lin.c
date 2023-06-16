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
void print_mat4(const mat4 m) {
  printf("{ ");
  for (int i = 0; i < 4; i++) {
    printf("{ %f %f %f %f } ", m[i][0], m[i][1], m[i][2], m[i][3]);
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
float radians(const float degrees) { return degrees * (M_PI / 180.0f); }

/**
 * ----------------------------------------------------------------------------
 */
void identity_mat4(mat4 m) {
  m[0][0] = 1;
  m[0][1] = 0;
  m[0][2] = 0;
  m[0][3] = 0;

  m[1][0] = 0;
  m[1][1] = 1;
  m[1][2] = 0;
  m[1][3] = 0;

  m[2][0] = 0;
  m[2][1] = 0;
  m[2][2] = 1;
  m[2][3] = 0;

  m[3][0] = 0;
  m[3][1] = 0;
  m[3][2] = 0;
  m[3][3] = 1;
}

/**
 * ----------------------------------------------------------------------------
 */
void normalize_vec3(vec3 vec) {
  float l = sqrt((vec[0] * vec[0]) + (vec[1] * vec[1]) + (vec[2] * vec[2]));
  vec[0] /= l;
  vec[1] /= l;
  vec[2] /= l;
}

/**
 * ----------------------------------------------------------------------------
 */
float dot_vec3(const vec3 v1, const vec3 v2) {
  return (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]);
}

/**
 * ----------------------------------------------------------------------------
 */
void copy_vec3(vec3 dest, const vec3 src) {
  dest[0] = src[0];
  dest[1] = src[1];
  dest[2] = src[2];
}

/**
 * ----------------------------------------------------------------------------
 */
int cmp_vec3(vec3 v1, vec3 v2) {
  for (int i = 0; i < 3; i++) {
    if (fabs(v1[i] - v2[i]) > FLT_EPSILON) {
      return 0;
    }
  }
  return 1;
}

/**
 * ----------------------------------------------------------------------------
 */
void copy_vec2(vec2 dest, const vec2 src) {
  dest[0] = src[0];
  dest[1] = src[1];
}

/**
 * ----------------------------------------------------------------------------
 */
int cmp_vec2(vec2 v1, vec2 v2) {
  for (int i = 0; i < 2; i++) {
    if (fabs(v1[i] - v2[i]) > FLT_EPSILON) {
      return 0;
    }
  }
  return 1;
}

/**
 * ----------------------------------------------------------------------------
 */
float dot_vec4(const vec4 v1, const vec4 v2) {
  return (v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]) + (v1[3] * v2[3]);
}
/**
 * ----------------------------------------------------------------------------
 */
void cross_vec3(vec3 dest, const vec3 v1, const vec3 v2) {
  dest[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
  dest[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
  dest[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
}

/**
 * ----------------------------------------------------------------------------
 */
void product_mat4(float dest[4][4], float m1[4][4], float m2[4][4]) {
  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0; j < 4; j++) {
      for (size_t k = 0; k < 4; k++) {
        dest[i][k] += m1[i][j] * m2[j][k];
      }
    }
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void product_vec4(float dest[4], float mat[4][4], float vec[4]) {
  for (size_t i = 0; i < 4; i++) {
    for (size_t j = 0; j < 4; j++) {
      dest[i] += mat[i][j] * vec[j];
    }
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void perspective(mat4 mat, const float fov, const float aspect_ratio,
                 const float near, const float far) {
  const float t = tan(fov / 2.0f);

  mat[0][0] = 1.0f / (aspect_ratio * t);
  mat[1][1] = 1.0f / t;
  mat[2][2] = -(far + near) / (far - near);
  mat[2][3] = -1.0f;
  mat[3][2] = -(2.0f * far * near) / (far - near);
}

/**
 * ----------------------------------------------------------------------------
 */
void look_at(mat4 mat, const vec3 eye, const vec3 ctr, const vec3 up) {
  vec3 f = {ctr[0] - eye[0], ctr[1] - eye[1], ctr[2] - eye[2]};
  normalize_vec3(f);

  vec3 s;
  cross_vec3(s, f, up);
  normalize_vec3(s);

  vec3 u;
  cross_vec3(u, s, f);

  identity_mat4(mat);

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
}

/**
 * ----------------------------------------------------------------------------
 */
void translate(mat4 m, const vec3 v) {
  m[3][0] += v[0];
  m[3][1] += v[1];
  m[3][2] += v[2];
}

/**
 * ----------------------------------------------------------------------------
 */
void create_rotation_matrix(float dest[4][4], float m[4][4], float axis[3],
                            float angle) {
  const float c = cos(angle);
  const float s = sin(angle);

  normalize_vec3(axis);

  mat4 rot = {0};
  rot[0][0] = c + (1.0f - c) * axis[0] * axis[0];
  rot[0][1] = (1.0f - c) * axis[0] * axis[1] + s * axis[2];
  rot[0][2] = (1.0f - c) * axis[0] * axis[2] - s * axis[1];
  rot[0][3] = 0.0f;

  rot[1][0] = (1.0f - c) * axis[1] * axis[0] - s * axis[2];
  rot[1][1] = c + (1.0f - c) * axis[1] * axis[1];
  rot[1][2] = (1.0f - c) * axis[1] * axis[2] + s * axis[0];
  rot[1][3] = 0.0f;

  rot[2][0] = (1.0f - c) * axis[2] * axis[0] + s * axis[1];
  rot[2][1] = (1.0f - c) * axis[2] * axis[1] - s * axis[0];
  rot[2][2] = c + (1.0f - c) * axis[2] * axis[2];
  rot[2][3] = 0.0f;

  rot[3][0] = 0.0f;
  rot[3][1] = 0.0f;
  rot[3][2] = 0.0f;
  rot[3][3] = 1.0f;

  product_mat4(dest, m, rot);
}

/**
 * ----------------------------------------------------------------------------
 */
void scale(float dest[4][4], float m[4][4], float v[3]) {
  mat4 temp = IDENTITY_MAT4_INITIALIZER;
  temp[0][0] = v[0];
  temp[1][1] = v[1];
  temp[2][2] = v[2];
  temp[3][0] = m[3][0];
  temp[3][1] = m[3][1];
  temp[3][2] = m[3][2];
  temp[3][3] = m[3][3];

  product_mat4(dest, m, temp);
}
