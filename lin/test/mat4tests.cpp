#include <gtest/gtest.h>
#include <mat4.h>

#include <iostream>
#include <sstream>

TEST(mat4tests, constructor) {
  Mat4 mat(1.0f);
  for (auto i = 0; i < 4; i++) {
    for (auto j = 0; j < 4; j++) {
      if (i == j) {
        ASSERT_FLOAT_EQ(mat[i][j], 1.0f);
      } else {
        ASSERT_FLOAT_EQ(mat[i][j], 0.0f);
      }
    }
  }
}

TEST(mat4tests, subscript_operator) {
  Mat4 mat(42.0f);
  auto row3 = mat[2];
  ASSERT_FLOAT_EQ(row3[0], mat[2][0]);
  ASSERT_FLOAT_EQ(row3[2], mat[2][2]);
  ASSERT_FLOAT_EQ(mat[3][3], 42.0f);
  mat[3][3] = -6.66f;
  ASSERT_FLOAT_EQ(mat[3][3], -6.66f);
}

TEST(mat4test, col) {
  Mat4 mat(42.0f);
  auto col = mat.col(2);
  ASSERT_FLOAT_EQ(col[0], 0.0f);
  ASSERT_FLOAT_EQ(col[1], 0.0f);
  ASSERT_FLOAT_EQ(col[2], 42.0f);
  ASSERT_FLOAT_EQ(col[3], 0.0f);
}

TEST(mat4tests, ostream) {
  Mat4 mat(1.0f);
  std::stringstream ss;
  ss << mat;
  ASSERT_EQ(ss.str(), std::string("{ { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, "
                                  "1, 0 }, { 0, 0, 0, 1 } }"));
}

TEST(mat4tests, product) {
  Mat4 m1(0.0f);
  Mat4 m2(0.0f);
  float x = 0.0f;
  for (auto i = 0; i < 4; i++) {
    for (auto j = 0; j < 4; j++) {
      m1[i][j] = x;
      m2[i][j] = x;
      x += 1.0f;
    }
  }
  auto result = m1 * m2;
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
