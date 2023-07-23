#include <gtest/gtest.h>

#include "component_table.hpp"

TEST(component_table_tests, placeholder) {
  xen::component_table<int> table;
  table.insert(0, 0);
  table.insert(10, 10);
  table.insert(5, 5);
  ASSERT_EQ(table[0], 0);
  ASSERT_EQ(table[1], 5);
  ASSERT_EQ(table[2], 10);
}

TEST(component_table_tests, iterator) {
  xen::component_table<float> table;
  table.insert(0, 0.0f);
  table.insert(10, 10.0f);
  table.insert(5, 5.0f);
  std::vector<float> exp = {0.0, 5.0f, 10.0f};
  auto it = table.begin();
  for (int i = 0; i < 3; i++)
  {
    ASSERT_EQ(*it, exp[i]);
    ++it;
  }

  ASSERT_TRUE(table.begin() < table.begin() + 1);
  ASSERT_TRUE(table.end() > table.begin() + 1);
}