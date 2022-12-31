#include <gtest/gtest.h>
#include <lin.h>

TEST(lintests, radians) {
  ASSERT_FLOAT_EQ(radians(3.122f), 0.054489179f);
}

TEST(lintests, identity_mat4) {
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

TEST(lintests, normalize_vec3) {
  vec3 v = {5, 2, -3};
  normalize_vec3(v);

  ASSERT_FLOAT_EQ(v[0],  0.8111071056538127f);
  ASSERT_FLOAT_EQ(v[1],  0.3244428422615251f);
  ASSERT_FLOAT_EQ(v[2], -0.4866642633922876f);
}

TEST(lintests, cross_vec3) {
  vec3 dest = {0};
  vec3 v1 = {1.0f, 2.0f, 3.0f};
  vec3 v2 = {1.0f, 5.0f, 7.0f};
  cross_vec3(dest, v1, v2);
  ASSERT_FLOAT_EQ(dest[0], -1.0f);
  ASSERT_FLOAT_EQ(dest[1], -4.0f);
  ASSERT_FLOAT_EQ(dest[2], 3.0f);
}

TEST(lintests, product_mat4) {
  mat4 m1 = {{0}};
  mat4 m2 = {{0}};

  float x = 0.0f;
  for (auto i = 0; i < 4; i++) {
    for (auto j = 0; j < 4; j++) {
      m1[i][j] = x;
      m2[i][j] = x;
      x += 1.0f;
    }
  }
  mat4 result = {{0}};
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

TEST(lintests, dot_vec3) {
  vec3 v1 = {1.0f, -3.2f, 0.0f};
  vec3 v2 = {5.4f, 3.2f, -5.0f};
  ASSERT_FLOAT_EQ(dot_vec3(v1, v2), -4.84);
}

TEST(lintests, dot_vec4) {
  vec4 v1 = {1.0f, -3.2f, 0.0f, 1.0f};
  vec4 v2 = {5.4f, 3.2f, -5.0f, -0.5f};
  ASSERT_FLOAT_EQ(dot_vec4(v1, v2), -5.34);
}

TEST(lintests, perspective) {
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

TEST(lintests, translate) {
  mat4 m;
  identity_mat4(m);
  vec3 v = {0.1f, 0.1f, 0.1f};
  translate(m, v);

  ASSERT_FLOAT_EQ(m[3][0], 0.1f);
  ASSERT_FLOAT_EQ(m[3][1], 0.1f);
  ASSERT_FLOAT_EQ(m[3][2], 0.1f);
}

TEST(lintests, rotate) {
  mat4 dest = {{0}};
  mat4 m = {{0}};
  identity_mat4(m);
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

TEST(lintests, look_at) {
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
