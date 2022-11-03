#include <gtest/gtest.h>
#include <lin.h>

#include "mat4.h"
#include "vec3.h"

TEST(lintests, radians) { ASSERT_FLOAT_EQ(lin::radians(3.122f), 0.054489179f); }

TEST(lintests, perspective) {
  auto pm = lin::perspective(45.0f, 0.1f, 100.0f, (800.0f / 600.0f));

  ASSERT_FLOAT_EQ(pm[0][0], 1.81066f);
  ASSERT_FLOAT_EQ(pm[0][1], 0.0f);
  ASSERT_FLOAT_EQ(pm[0][2], 0.0f);
  ASSERT_FLOAT_EQ(pm[0][3], 0.0f);

  ASSERT_FLOAT_EQ(pm[1][0], 0.0f);
  ASSERT_FLOAT_EQ(pm[1][1], 2.4142134f);
  ASSERT_FLOAT_EQ(pm[1][2], 0.0f);
  ASSERT_FLOAT_EQ(pm[1][3], 0.0f);

  ASSERT_FLOAT_EQ(pm[2][0], 0.0f);
  ASSERT_FLOAT_EQ(pm[2][1], 0.0f);
  ASSERT_FLOAT_EQ(pm[2][2], -1.002002f);
  ASSERT_FLOAT_EQ(pm[2][3], -1.0f);

  ASSERT_FLOAT_EQ(pm[3][0], 0.0f);
  ASSERT_FLOAT_EQ(pm[3][1], 0.0f);
  ASSERT_FLOAT_EQ(pm[3][2], -0.2002002f);
  ASSERT_FLOAT_EQ(pm[3][3], 0.0f);
}

TEST(lintests, translate) {
  auto t = lin::translate(Mat4(1.0f), Vec3(0.1f, 0.1f, 0.1f));
  ASSERT_FLOAT_EQ(t[3][0], 0.1f);
  ASSERT_FLOAT_EQ(t[3][1], 0.1f);
  ASSERT_FLOAT_EQ(t[3][2], 0.1f);
}
