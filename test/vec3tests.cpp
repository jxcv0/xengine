#include <gtest/gtest.h>
#include <vec3.h>

TEST(vec3tests, default_values) {
  Vec3 vec;
  ASSERT_FLOAT_EQ(vec.x(), 0.0f);
  ASSERT_FLOAT_EQ(vec.y(), 0.0f);
  ASSERT_FLOAT_EQ(vec.z(), 0.0f);
}

TEST(vec3tests, scalar) {
  Vec3 vec(1.0f, 2.0f, 3.0f);
  auto result = vec * 0.1f;
  ASSERT_FLOAT_EQ(result.x(), 0.1f);
  ASSERT_FLOAT_EQ(result.y(), 0.2f);
  ASSERT_FLOAT_EQ(result.z(), 0.3f);
}

TEST(vec3tests, scalar_in_place) {
  Vec3 vec(1.0f, 2.0f, 3.0f);
  vec *= 0.1f;
  ASSERT_FLOAT_EQ(vec.x(), 0.1f);
  ASSERT_FLOAT_EQ(vec.y(), 0.2f);
  ASSERT_FLOAT_EQ(vec.z(), 0.3f);
}

TEST(vec3tests, normalize) {
  Vec3 vec(1.0f, 1.0f, 1.0f);
  auto norm = vec.normalize();
  ASSERT_FLOAT_EQ(norm.x(), 0.57735026f);
  ASSERT_FLOAT_EQ(norm.y(), 0.57735026f);
  ASSERT_FLOAT_EQ(norm.z(), 0.57735026f);
}

TEST(vec3tests, add) {
  Vec3 v1(1.0f, -3.2f, 0.0f);
  Vec3 v2(5.4f, 3.2f, -5.0f);
  Vec3 result = v1 + v2;
  ASSERT_FLOAT_EQ(result.x(), 6.4f);
  ASSERT_FLOAT_EQ(result.y(), 0.0f);
  ASSERT_FLOAT_EQ(result.z(), -5.0f);
}

TEST(vec3tests, add_in_place) {
  Vec3 v1(1.0f, -3.2f, 0.0f);
  Vec3 v2(5.4f, 3.2f, -5.0f);
  v1 += v2;
  ASSERT_FLOAT_EQ(v1.x(), 6.4f);
  ASSERT_FLOAT_EQ(v1.y(), 0.0f);
  ASSERT_FLOAT_EQ(v1.z(), -5.0f);
}

TEST(vec3tests, subtract) {
  Vec3 v1(1.0f, -3.2f, 0.0f);
  Vec3 v2(5.4f, 3.2f, -5.0f);
  Vec3 result = v1 - v2;
  ASSERT_FLOAT_EQ(result.x(), -4.4f);
  ASSERT_FLOAT_EQ(result.y(), -6.4f);
  ASSERT_FLOAT_EQ(result.z(), 5.0f);
}

TEST(vec3tests, subtract_in_place) {
  Vec3 v1(1.0f, -3.2f, 0.0f);
  Vec3 v2(5.4f, 3.2f, -5.0f);
  v1 -= v2;
  ASSERT_FLOAT_EQ(v1.x(), -4.4f);
  ASSERT_FLOAT_EQ(v1.y(), -6.4f);
  ASSERT_FLOAT_EQ(v1.z(), 5.0f);
}

TEST(vec3test, dot) {
  Vec3 v1(1.0f, -3.2f, 0.0f);
  Vec3 v2(5.4f, 3.2f, -5.0f);
  ASSERT_FLOAT_EQ(dot(v1, v2), -4.84);
}

TEST(vec3tests, cross) {
  Vec3 v1(1.0f, 2.0f, 3.0f);
  Vec3 v2(1.0f, 5.0f, 7.0f);
  auto result = v1 * v2;
  ASSERT_FLOAT_EQ(result.x(), -1.0f);
  ASSERT_FLOAT_EQ(result.y(), -4.0f);
  ASSERT_FLOAT_EQ(result.z(), 3.0f);
}

TEST(vec3tests, cross_in_place) {
  Vec3 v1(1.0f, 2.0f, 3.0f);
  Vec3 v2(1.0f, 5.0f, 7.0f);
  v1 *= v2;
  ASSERT_FLOAT_EQ(v1.x(), -1.0f);
  ASSERT_FLOAT_EQ(v1.y(), -4.0f);
  ASSERT_FLOAT_EQ(v1.z(), 3.0f);
}
