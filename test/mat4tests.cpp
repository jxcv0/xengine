#include <cstddef>
#include <gtest/gtest.h>
#include <mat4.h>

#include <iostream>

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
  Mat4 mat(1.0f);
  auto row3 = mat[2];
  ASSERT_FLOAT_EQ(row3[0], mat[2][0]);
  ASSERT_FLOAT_EQ(row3[2], mat[2][2]);
}

TEST(mat4tests, ostream) {
  Mat4 mat(1.0f);
  std::cout << "ostream test: " << mat << "\n";
}

// TEST(mat4test, cross) {
//   Mat4 m1(0.0f);
//   Mat4 m2(0.0f);
//   float x = 0.0f;
//   for (auto i = 0; i < 4; i++) {
//     for (auto j = 0; j < 4; j++) {
//       m1[i][j] = x;
//       m1[i][j] = x;
//       x += 1.0f;
//     }
//   }
//   std::cout << m1 << "\n";
// }
