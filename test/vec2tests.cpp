#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vec2.h>

#include <iostream>

TEST(vec2tests, ostream) {
  Vec2 vec(1.05f, 0.833f);
  std::stringstream ss;
  ss << vec;
  ASSERT_EQ(ss.str(), std::string("{ 1.05, 0.833 }"));
}

TEST(vec2tests, default_values) {
  Vec2 vec;
  ASSERT_FLOAT_EQ(vec.x(), 0.0f);
  ASSERT_FLOAT_EQ(vec.y(), 0.0f);
}

TEST(vec2tests, copy_construction) {
  Vec2 vec(1.05f, 0.833f);
  auto other = vec;
  ASSERT_FLOAT_EQ(other.x(), 1.05f);
  ASSERT_FLOAT_EQ(other.y(), 0.833f);
  ASSERT_EQ(vec, other);
}
