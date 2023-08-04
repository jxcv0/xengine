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

TEST(table_tests, iterate_two_tables) {
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

  iterate_tables(dest, src, [](int &i1, int &i2) { i1 += i2; });

  ASSERT_EQ(dest[0], 20);
  ASSERT_EQ(dest[1], 20);
  ASSERT_EQ(dest[2], 60);
  ASSERT_EQ(dest[3], 10);
}

TEST(table_tests, iterate_three_tables) {
  xen::table<int> t1;
  t1.insert(1);
  t1.insert(2);
  t1.insert(3);
  t1.insert(5);

  xen::table<int> t2;
  t2.insert(0, 0);
  t2.insert(1, 10);
  t2.insert(3, 30);
  t2.insert(6, 1000);

  xen::table<int> t3;
  t3.insert(1, 2);
  t3.insert(3, 2);

  iterate_tables(t1, t2, t3, [](int &i1, int &i2, int &i3) { i1 = i2 * i3; });

  ASSERT_EQ(t1[0], 20);
  ASSERT_EQ(t1[1], 0);
  ASSERT_EQ(t1[2], 60);
  ASSERT_EQ(t1[3], 0);
}
