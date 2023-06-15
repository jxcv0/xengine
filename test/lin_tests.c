#include <assert.h>
#include <float.h>
#include <libgen.h>
#include <stdio.h>
#include <string.h>

#include "lin.h"
#include "test.h"

void tst_radians(void) {
  TEST();
  ASSERT_FLOAT_EQ(radians(3.122f), 0.054489179f);
}

void tst_identity_mat4(void) {
  TEST();
  mat4 m;
  identity_mat4(m);
  ASSERT_FLOAT_EQ(m[0][0], 1);
  ASSERT_FLOAT_EQ(m[0][1], 0);
  ASSERT_FLOAT_EQ(m[0][2], 0);
  ASSERT_FLOAT_EQ(m[0][3], 0);

  ASSERT_FLOAT_EQ(m[1][0], 0);
  ASSERT_FLOAT_EQ(m[1][1], 1);
  ASSERT_FLOAT_EQ(m[1][2], 0);
  ASSERT_FLOAT_EQ(m[1][3], 0);

  ASSERT_FLOAT_EQ(m[2][0], 0);
  ASSERT_FLOAT_EQ(m[2][1], 0);
  ASSERT_FLOAT_EQ(m[2][2], 1);
  ASSERT_FLOAT_EQ(m[2][3], 0);

  ASSERT_FLOAT_EQ(m[3][0], 0);
  ASSERT_FLOAT_EQ(m[3][1], 0);
  ASSERT_FLOAT_EQ(m[3][2], 0);
  ASSERT_FLOAT_EQ(m[3][3], 1);
}

void tst_normalize_vec(void) {
  TEST();
  vec3 v = {5, 2, -3};
  normalize_vec3(v);

  ASSERT_FLOAT_EQ(v[0], 0.8111071056538127f);
  ASSERT_FLOAT_EQ(v[1], 0.3244428422615251f);
  ASSERT_FLOAT_EQ(v[2], -0.4866642633922876f);
}

void tst_cross_vec3(void) {
  TEST();
  vec3 dest = {0};
  vec3 v1 = {1.0f, 2.0f, 3.0f};
  vec3 v2 = {1.0f, 5.0f, 7.0f};
  cross_vec3(dest, v1, v2);
  ASSERT_FLOAT_EQ(dest[0], -1.0f);
  ASSERT_FLOAT_EQ(dest[1], -4.0f);
  ASSERT_FLOAT_EQ(dest[2], 3.0f);
}

void tst_product_mat4(void) {
  TEST();
  mat4 m1 = {0};
  mat4 m2 = {0};

  float x = 0.0f;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      m1[i][j] = x;
      m2[i][j] = x;
      x += 1.0f;
    }
  }

  mat4 result = {0};
  product_mat4(result, m1, m2);
  ASSERT_FLOAT_EQ(result[0][0], 56.0f);
  ASSERT_FLOAT_EQ(result[0][1], 62.0f);
  ASSERT_FLOAT_EQ(result[0][2], 68.0f);
  ASSERT_FLOAT_EQ(result[0][3], 74.0f);
  ASSERT_FLOAT_EQ(result[1][0], 152.0f);
  ASSERT_FLOAT_EQ(result[1][1], 174.0f);
  ASSERT_FLOAT_EQ(result[1][2], 196.0f);
  ASSERT_FLOAT_EQ(result[1][3], 218.0f);
  ASSERT_FLOAT_EQ(result[2][0], 248.0f);
  ASSERT_FLOAT_EQ(result[2][1], 286.0f);
  ASSERT_FLOAT_EQ(result[2][2], 324.0f);
  ASSERT_FLOAT_EQ(result[2][3], 362.0f);
  ASSERT_FLOAT_EQ(result[3][0], 344.0f);
  ASSERT_FLOAT_EQ(result[3][1], 398.0f);
  ASSERT_FLOAT_EQ(result[3][2], 452.0f);
  ASSERT_FLOAT_EQ(result[3][3], 506.0);
}

void tst_product_vec4(void) {
  TEST();
  float mat[4][4] = {0};

  float x = 0.0f;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      mat[i][j] = x;
      x += 1.0f;
    }
  }

  float vec[4] = {1.0f, 2.0f, 3.0f, 4.0f};
  float result[4] = {0};
  product_vec4(result, mat, vec);
  ASSERT_FLOAT_EQ(result[0], 20.0f);
  ASSERT_FLOAT_EQ(result[1], 60.0f);
  ASSERT_FLOAT_EQ(result[2], 100.0f);
  ASSERT_FLOAT_EQ(result[3], 140.0f);
}

void tst_dot_vec3(void) {
  TEST();
  vec3 v1 = {1.0f, -3.2f, 0.0f};
  vec3 v2 = {5.4f, 3.2f, -5.0f};
  ASSERT_FLOAT_EQ(dot_vec3(v1, v2), -4.84);
}

void tst_dot_vec4(void) {
  TEST();
  vec4 v1 = {1.0f, -3.2f, 0.0f, 1.0f};
  vec4 v2 = {5.4f, 3.2f, -5.0f, -0.5f};
  ASSERT_FLOAT_EQ(dot_vec4(v1, v2), -5.34);
}

void tst_perspective(void) {
  TEST();
  mat4 m = {{0}};
  perspective(m, radians(45.0f), (800.0f / 600.0f), 0.1f, 100.0f);

  ASSERT_FLOAT_EQ(m[0][0], 1.81066f);
  ASSERT_FLOAT_EQ(m[0][1], 0.0f);
  ASSERT_FLOAT_EQ(m[0][2], 0.0f);
  ASSERT_FLOAT_EQ(m[0][3], 0.0f);

  ASSERT_FLOAT_EQ(m[1][0], 0.0f);
  ASSERT_FLOAT_EQ(m[1][1], 2.4142134f);
  ASSERT_FLOAT_EQ(m[1][2], 0.0f);
  ASSERT_FLOAT_EQ(m[1][3], 0.0f);

  ASSERT_FLOAT_EQ(m[2][0], 0.0f);
  ASSERT_FLOAT_EQ(m[2][1], 0.0f);
  ASSERT_FLOAT_EQ(m[2][2], -1.002002f);
  ASSERT_FLOAT_EQ(m[2][3], -1.0f);

  ASSERT_FLOAT_EQ(m[3][0], 0.0f);
  ASSERT_FLOAT_EQ(m[3][1], 0.0f);
  ASSERT_FLOAT_EQ(m[3][2], -0.2002002f);
  ASSERT_FLOAT_EQ(m[3][3], 0.0f);
}

void tst_translate(void) {
  TEST();
  mat4 m;
  identity_mat4(m);
  vec3 v = {0.1f, 0.1f, 0.1f};
  translate(m, v);

  ASSERT_FLOAT_EQ(m[3][0], 0.1f);
  ASSERT_FLOAT_EQ(m[3][1], 0.1f);
  ASSERT_FLOAT_EQ(m[3][2], 0.1f);
}

void tst_rotate1(void) {
  TEST();
  mat4 dest = {0};
  mat4 m = IDENTITY_MAT4_INITIALIZER;
  vec3 v = {0.0f, 1.0f, 0.0f};
  rotate(dest, m, v, radians(30.0f));

  ASSERT_FLOAT_EQ(dest[0][0], 0.86602539f);
  ASSERT_FLOAT_EQ(dest[0][1], 0.0f);
  ASSERT_FLOAT_EQ(dest[0][2], -0.5f);
  ASSERT_FLOAT_EQ(dest[0][3], 0.0f);

  ASSERT_FLOAT_EQ(dest[1][0], 0.0f);
  ASSERT_FLOAT_EQ(dest[1][1], 1.0f);
  ASSERT_FLOAT_EQ(dest[1][2], 0.0f);
  ASSERT_FLOAT_EQ(dest[1][3], 0.0f);

  ASSERT_FLOAT_EQ(dest[2][0], 0.5f);
  ASSERT_FLOAT_EQ(dest[2][1], 0.0f);
  ASSERT_FLOAT_EQ(dest[2][2], 0.86602539f);
  ASSERT_FLOAT_EQ(dest[2][3], 0.0f);

  ASSERT_FLOAT_EQ(dest[3][0], 0.0f);
  ASSERT_FLOAT_EQ(dest[3][1], 0.0f);
  ASSERT_FLOAT_EQ(dest[3][2], 0.0f);
  ASSERT_FLOAT_EQ(dest[3][3], 1.0f);
}

void tst_rotate2(void) {
  TEST();
  float A[4] = {1.0f, 0.0f, 0.0f, 1.0f};

  float rotation[4][4] = {0};
  float ra[3] = {0.0f, 0.0f, 1.0f};

  rotate(rotation, IDENTITY_MAT4, ra, radians(-90.0f));

  float result[4] = {0};
  product_vec4(result, rotation, A);
  ASSERT_FLOAT_EQ(result[0], 0.0f);
  ASSERT_FLOAT_EQ(result[1], 1.0f);
  ASSERT_FLOAT_EQ(result[2], 0.0f);
  ASSERT_FLOAT_EQ(result[3], 1.0f);
}

void tst_look_at(void) {
  TEST();
  vec3 eye = {3.0f, 3.0f, 3.0f};
  vec3 ctr = {1.0f, 0.0f, 1.0f};
  vec3 c = {eye[0] + ctr[0], eye[1] + ctr[1], eye[2] + ctr[2]};
  vec3 up = {0.0f, 1.0f, 0.0f};
  mat4 m = {{0}};

  look_at(m, eye, c, up);
  ASSERT_FLOAT_EQ(m[0][0], -0.707107f);
  ASSERT_FLOAT_EQ(m[0][1], 0.0f);
  ASSERT_FLOAT_EQ(m[0][2], -0.707107f);
  ASSERT_FLOAT_EQ(m[0][3], 0.0f);
  ASSERT_FLOAT_EQ(m[1][0], 0.0f);
  ASSERT_FLOAT_EQ(m[1][1], 1.0f);
  ASSERT_FLOAT_EQ(m[1][2], -0.0f);
  ASSERT_FLOAT_EQ(m[1][3], 0.0f);
  ASSERT_FLOAT_EQ(m[2][0], 0.707107f);
  ASSERT_FLOAT_EQ(m[2][1], -0.0f);
  ASSERT_FLOAT_EQ(m[2][2], -0.707107f);
  ASSERT_FLOAT_EQ(m[2][3], 0.0f);
  ASSERT_FLOAT_EQ(m[3][0], -0.0f);
  ASSERT_FLOAT_EQ(m[3][1], -3.0f);
  ASSERT_FLOAT_EQ(m[3][2], 4.24264f);
  ASSERT_FLOAT_EQ(m[3][3], 1.0f);
}

void tst_scale(void) {
  TEST();
  mat4 m = {0};
  m[0][2] = 1.032f;
  m[2][1] = 0.032f;
  m[3][0] = 1.0f;
  m[3][2] = 0.888f;
  vec3 v = {0.0f, 2.0f, 3.0f};

  mat4 result = {0};
  scale(result, m, v);

  ASSERT_FLOAT_EQ(result[0][0], 0.000000);
  ASSERT_FLOAT_EQ(result[0][1], 0.000000);
  ASSERT_FLOAT_EQ(result[0][2], 3.096000);
  ASSERT_FLOAT_EQ(result[0][3], 0.000000);
  ASSERT_FLOAT_EQ(result[1][0], 0.000000);
  ASSERT_FLOAT_EQ(result[1][1], 0.000000);
  ASSERT_FLOAT_EQ(result[1][2], 0.000000);
  ASSERT_FLOAT_EQ(result[1][3], 0.000000);
  ASSERT_FLOAT_EQ(result[2][0], 0.000000);
  ASSERT_FLOAT_EQ(result[2][1], 0.064000);
  ASSERT_FLOAT_EQ(result[2][2], 0.000000);
  ASSERT_FLOAT_EQ(result[2][3], 0.000000);
  ASSERT_FLOAT_EQ(result[3][0], 0.000000);
  ASSERT_FLOAT_EQ(result[3][1], 0.000000);
  ASSERT_FLOAT_EQ(result[3][2], 2.664000);
  ASSERT_FLOAT_EQ(result[3][3], 0.000000);
}

int main() {
  TEST_BEGIN();
  tst_radians();
  tst_identity_mat4();
  tst_normalize_vec();
  tst_cross_vec3();
  tst_product_mat4();
  tst_product_vec4();
  tst_dot_vec3();
  tst_dot_vec4();
  tst_perspective();
  tst_translate();
  tst_rotate1();
  tst_rotate2();
  tst_look_at();
  tst_scale();
  TEST_END();
  return 0;
}
