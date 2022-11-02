#include <gtest/gtest.h>
#include <sstream>
#include <vec4.h>

#include <iostream>

TEST(vec4tests, ostream) {
  Vec4 vec(1.05f, 0.833f, 30.0f, -1.0f);
  std::stringstream ss;
  ss << vec;
  ASSERT_EQ(ss.str(), std::string("{ 1.05, 0.833, 30, -1 }"));
}

TEST(vec4tests, default_values) {
  Vec4 vec;
  ASSERT_FLOAT_EQ(vec.x(), 0.0f);
  ASSERT_FLOAT_EQ(vec.y(), 0.0f);
  ASSERT_FLOAT_EQ(vec.z(), 0.0f);
  ASSERT_FLOAT_EQ(vec.w(), 0.0f);
}

TEST(vec4tests, subscript) {
  Vec4 vec(1.0f, 2.0f, 3.0f, 4.0f);
  ASSERT_FLOAT_EQ(vec[1], 2.0f);
  vec[2] = 10.0f;
  ASSERT_FLOAT_EQ(vec[2], 10.0f);
}

TEST(vec4tests, copy_construction) {
  Vec4 vec(1.05f, 0.833f, 30.0f, -1.0f);
  auto other = vec;
  ASSERT_FLOAT_EQ(other.x(), 1.05f);
  ASSERT_FLOAT_EQ(other.y(), 0.833f);
  ASSERT_FLOAT_EQ(other.z(), 30.0f);
  ASSERT_FLOAT_EQ(other.w(), -1.0f);
  ASSERT_EQ(vec, other);
}

TEST(vec4tests, scalar) {
  Vec4 vec(1.0f, 2.0f, 3.0f, -2.5f);
  auto result = vec * 0.1f;
  ASSERT_FLOAT_EQ(result.x(), 0.1f);
  ASSERT_FLOAT_EQ(result.y(), 0.2f);
  ASSERT_FLOAT_EQ(result.z(), 0.3f);
  ASSERT_FLOAT_EQ(result.w(), -0.25f);
}

TEST(vec4tests, scalar_in_place) {
  Vec4 vec(1.0f, 2.0f, 3.0f, -2.5f);
  vec *= 0.1f;
  ASSERT_FLOAT_EQ(vec.x(), 0.1f);
  ASSERT_FLOAT_EQ(vec.y(), 0.2f);
  ASSERT_FLOAT_EQ(vec.z(), 0.3f);
  ASSERT_FLOAT_EQ(vec.w(), -0.25f);
}

TEST(vec4tests, normalize) {
  Vec4 vec(1.0f, 1.0f, 1.0f, 1.0f);
  auto norm = vec.normalize();
  ASSERT_FLOAT_EQ(norm.x(), 0.5);
  ASSERT_FLOAT_EQ(norm.y(), 0.5);
  ASSERT_FLOAT_EQ(norm.z(), 0.5);
  ASSERT_FLOAT_EQ(norm.w(), 0.5);
}

TEST(vec4tests, add) {
  Vec4 v1(1.0f, -3.2f, 0.0f, -1.0f);
  Vec4 v2(5.4f, 3.2f, -5.0f, -1.0f);
  Vec4 result = v1 + v2;
  ASSERT_FLOAT_EQ(result.x(), 6.4f);
  ASSERT_FLOAT_EQ(result.y(), 0.0f);
  ASSERT_FLOAT_EQ(result.z(), -5.0f);
  ASSERT_FLOAT_EQ(result.w(), -2.0f);
}

TEST(vec4tests, add_in_place) {
  Vec4 v1(1.0f, -3.2f, 0.0f, -1.0f);
  Vec4 v2(5.4f, 3.2f, -5.0f, -1.0f);
  v1 += v2;
  ASSERT_FLOAT_EQ(v1.x(), 6.4f);
  ASSERT_FLOAT_EQ(v1.y(), 0.0f);
  ASSERT_FLOAT_EQ(v1.z(), -5.0f);
  ASSERT_FLOAT_EQ(v1.w(), -2.0f);
}

TEST(vec4tests, subtract) {
  Vec4 v1(1.0f, -3.2f, 0.0f, -1.0f);
  Vec4 v2(5.4f, 3.2f, -5.0f, -1.0f);
  Vec4 result = v1 - v2;
  ASSERT_FLOAT_EQ(result.x(), -4.4f);
  ASSERT_FLOAT_EQ(result.y(), -6.4f);
  ASSERT_FLOAT_EQ(result.z(), 5.0f);
  ASSERT_FLOAT_EQ(result.w(), 0.0f);
}

TEST(vec4tests, subtract_in_place) {
  Vec4 v1(1.0f, -3.2f, 0.0f, -1.0f);
  Vec4 v2(5.4f, 3.2f, -5.0f, -1.0f);
  v1 -= v2;
  ASSERT_FLOAT_EQ(v1.x(), -4.4f);
  ASSERT_FLOAT_EQ(v1.y(), -6.4f);
  ASSERT_FLOAT_EQ(v1.z(), 5.0f);
  ASSERT_FLOAT_EQ(v1.w(), 0.0f);
}

TEST(vec4tests, dot) {
  Vec4 v1(1.0f, -3.2f, 0.0f, 1.0f);
  Vec4 v2(5.4f, 3.2f, -5.0f, -0.5f);
  ASSERT_FLOAT_EQ(dot(v1, v2), -5.34);
}
