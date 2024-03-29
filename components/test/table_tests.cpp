#include <gtest/gtest.h>

#include "table.hpp"

TEST(component_table_tests, insert) {
  xen::table<int> table;
  table.insert(0, 0);
  table.insert(10, 10);
  table.insert(5, 5);
  ASSERT_EQ(table[0], 0);
  ASSERT_EQ(table[1], 5);
  ASSERT_EQ(table[2], 10);
}

TEST(component_table_tests, begin_and_end) {
  xen::table<float> table;
  table.insert(0, 0.0f);
  table.insert(10, 10.0f);
  table.insert(5, 5.0f);
  auto begin = table.begin();
  ASSERT_FLOAT_EQ(*begin, 0.0f);
  auto last = table.end() - 1;
  ASSERT_FLOAT_EQ(*last, 10.0f);
}

TEST(component_table_tests, iterator) {
  xen::table<float> table;
  table.insert(0, 0.0f);
  table.insert(10, 10.0f);
  table.insert(5, 5.0f);
  std::vector<float> exp = {0.0, 5.0f, 10.0f};
  auto it = table.begin();
  for (int i = 0; i < 3; i++) {
    ASSERT_EQ(*it, exp[i]);
    ++it;
  }

  ASSERT_TRUE(table.begin() < table.begin() + 1);
  ASSERT_TRUE(table.end() > table.begin() + 1);
  ASSERT_TRUE(table.cbegin() < table.cbegin() + 1);
  ASSERT_TRUE(table.cend() > table.cbegin() + 1);
}

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

  auto dest_it = dest.begin();
  auto src_it = src.begin();
  while (dest_it != dest.end() && src_it != src.end()) {
    if (dest_it == src_it) {
      *dest_it += *src_it;
      ++dest_it;
      ++src_it;
    } else {
      if (dest_it < src_it) ++dest_it;
      if (dest_it > src_it) ++src_it;
    }
  }

  ASSERT_EQ(dest[0], 20);
  ASSERT_EQ(dest[1], 20);
  ASSERT_EQ(dest[2], 60);
  ASSERT_EQ(dest[3], 10);
}