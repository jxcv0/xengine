#include <gtest/gtest.h>

#include "component_table.hpp"

TEST(component_table_tests, placeholder) {
  xen::component_table<int> position_table;
  position_table.insert(0, 0);
  position_table.insert(10, 10);
  position_table.insert(5, 5);
  ASSERT_EQ(position_table[0], 0);
  ASSERT_EQ(position_table[1], 5);
  ASSERT_EQ(position_table[2], 10);
}