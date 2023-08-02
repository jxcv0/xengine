#include <gtest/gtest.h>

#include "table.hpp"

TEST(table_tests, insert_sorted) {
  struct C {
    int i;
  };
  xen::table<C> transform_table{};
  transform_table.insert(0, {1});
  ASSERT_EQ(transform_table[0].i, 1);
  transform_table.insert(3, {4});
  ASSERT_EQ(transform_table[0].i, 1);
  ASSERT_EQ(transform_table[1].i, 4);
  transform_table.insert(1, {2});
  ASSERT_EQ(transform_table[0].i, 1);
  ASSERT_EQ(transform_table[1].i, 2);
  ASSERT_EQ(transform_table[2].i, 4);
  transform_table.remove(1);
  ASSERT_EQ(transform_table[0].i, 1);
  ASSERT_EQ(transform_table[1].i, 4);
}

TEST(table_tests, iterate_tables) {
  xen::table<int> dest;
  dest.insert(1, 10);
  dest.insert(2, 20);
  dest.insert(3, 30);
  dest.insert(5, 10);

  xen::table<int> src;
  src.insert(0, 0);
  src.insert(1, 10);
  src.insert(3, 30);
  src.insert(6, 1000);

  iterate_tables(dest, src, [](int& i1, int& i2) { i1 += i2; });

  ASSERT_EQ(dest[0], 20);
  ASSERT_EQ(dest[1], 20);
  ASSERT_EQ(dest[2], 60);
  ASSERT_EQ(dest[3], 10);
}
