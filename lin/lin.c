#include "lin.h"

#include <stdio.h>

/**
 * ----------------------------------------------------------------------------
 */
void normalize_vf3(vec3 vec) {
  float l = sqrt((vec[0] * vec[0]) + (vec[1] * vec[1]) + (vec[2] * vec[2]));
  vec[0] /= l;
  vec[1] /= l;
  vec[2] /= l;
}

/**
 * ----------------------------------------------------------------------------
 */
float radians(const float degrees) { return degrees * (M_PI / 180.0f); }

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
  (void)mat;
  (void)eye;
  (void)ctr;
  (void)up;
  /*
auto f = (ctr - eye).normalize();
auto s = (f * up).normalize();
auto u = s * f;

Mat4 mat(1.0f);
mat[0][0] = s[0];
mat[1][0] = s[1];
mat[2][0] = s[2];

mat[0][1] = u[0];
mat[1][1] = u[1];
mat[2][1] = u[2];

mat[0][2] = -f[0];
mat[1][2] = -f[1];
mat[2][2] = -f[2];

mat[3][0] = -dot(s, eye);
mat[3][1] = -dot(u, eye);
mat[3][2] = dot(f, eye);

*/
}
