#include <gtest/gtest.h>

#include "intersection.hpp"

TEST(component_table_tests, use_case) {
  xen::table<int> dest;
  dest.insert(1, 10);
  dest.insert(2, 20);
  dest.insert(3, 30);
  dest.insert(5, 10);

  xen::table<int> src;
  src.insert(0, 0);
  src.insert(1, 10);
  src.insert(3, 30);

  xen::intersection set{dest, src};
  std::for_each(set.begin(), set.end(), [](auto& [d, s]) { d += s; });

  ASSERT_EQ(dest[0], 20);
  ASSERT_EQ(dest[1], 20);
  ASSERT_EQ(dest[2], 60);
  ASSERT_EQ(dest[3], 10);
}