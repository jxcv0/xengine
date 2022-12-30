#include <gtest/gtest.h>
#include <lin.h>

TEST(lintests, radians) {
    ASSERT_FLOAT_EQ(radians(3.122f), 0.054489179f);
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

/*
TEST(lintests, translate) {
  auto t = lin::translate(Mat4(1.0f), Vec3(0.1f, 0.1f, 0.1f));
  ASSERT_FLOAT_EQ(t[3][0], 0.1f);
  ASSERT_FLOAT_EQ(t[3][1], 0.1f);
  ASSERT_FLOAT_EQ(t[3][2], 0.1f);
}

TEST(lintests, rotate) {
  auto r = lin::rotate(Mat4(1.0f), Vec3(0.0f, 1.0f, 0.0f), lin::radians(30.0f));
  ASSERT_FLOAT_EQ(r[0][0], 0.86602539f);
  ASSERT_FLOAT_EQ(r[0][1], 0.0f);
  ASSERT_FLOAT_EQ(r[0][2], -0.5f);
  ASSERT_FLOAT_EQ(r[0][3], 0.0f);
  ASSERT_FLOAT_EQ(r[1][0], 0.0f);
  ASSERT_FLOAT_EQ(r[1][1], 1.0f);
  ASSERT_FLOAT_EQ(r[1][2], 0.0f);
  ASSERT_FLOAT_EQ(r[1][3], 0.0f);
  ASSERT_FLOAT_EQ(r[2][0], 0.5f);
  ASSERT_FLOAT_EQ(r[2][1], 0.0f);
  ASSERT_FLOAT_EQ(r[2][2], 0.86602539f);
  ASSERT_FLOAT_EQ(r[2][3], 0.0f);
  ASSERT_FLOAT_EQ(r[3][0], 0.0f);
  ASSERT_FLOAT_EQ(r[3][1], 0.0f);
  ASSERT_FLOAT_EQ(r[3][2], 0.0f);
  ASSERT_FLOAT_EQ(r[3][3], 1.0f);
}

TEST(lintests, look_at) {
  Vec3 eye(3.0f, 3.0f, 3.0f);
  Vec3 ctr(1.0f, 0.0f, 1.0f);
  Vec3 up(0.0f, 1.0f, 0.0f);

  auto a = lin::look_at(eye, eye + ctr, up);
  ASSERT_FLOAT_EQ(a[0][0], -0.707107f);
  ASSERT_FLOAT_EQ(a[0][1], 0.0f);
  ASSERT_FLOAT_EQ(a[0][2], -0.707107f);
  ASSERT_FLOAT_EQ(a[0][3], 0.0f);
  ASSERT_FLOAT_EQ(a[1][0], 0.0f);
  ASSERT_FLOAT_EQ(a[1][1], 1.0f);
  ASSERT_FLOAT_EQ(a[1][2], -0.0f);
  ASSERT_FLOAT_EQ(a[1][3], 0.0f);
  ASSERT_FLOAT_EQ(a[2][0], 0.707107f);
  ASSERT_FLOAT_EQ(a[2][1], -0.0f);
  ASSERT_FLOAT_EQ(a[2][2], -0.707107f);
  ASSERT_FLOAT_EQ(a[2][3], 0.0f);
  ASSERT_FLOAT_EQ(a[3][0], -0.0f);
  ASSERT_FLOAT_EQ(a[3][1], -3.0f);
  ASSERT_FLOAT_EQ(a[3][2], 4.24264f);
  ASSERT_FLOAT_EQ(a[3][3], 1.0f);
}
*/
