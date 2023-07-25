#include <gtest/gtest.h>

#include "ecs.hpp"

TEST(archetype_tests, has_types) {
  xen::archetype_interface *arch = new xen::archetype<int, float, char>;
  bool b = arch->has_components<int, float>();
  ASSERT_TRUE(b);
  b = arch->has_components<int, char>();
  ASSERT_TRUE(b);
  b = arch->has_components<float, char>();
  ASSERT_TRUE(b);
  b = arch->has_components<double, char>();
  ASSERT_FALSE(b);
}
