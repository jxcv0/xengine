#include "lin.h"

#include <stdio.h>

/**
 * ----------------------------------------------------------------------------
 */
void print_mat4(const mat4 mat) {
  printf("{ ");
  for (int i = 0; i < 4; i++) {
    printf("{ ");
    for (int j = 0; j < 4; j++) {
      printf("%.6f ", mat[i][j]);
    }
    printf(" }");
  }
  printf(" }\n");
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
