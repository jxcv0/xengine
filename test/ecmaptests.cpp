#include <cassert>
#include <gtest/gtest.h>
#include <ecmap.h>
#include <stdexcept>

TEST(ecmaptests, assign_and_find) {
  ECMap<10, int, float> map;
  map.assign(10, 0.33f);
  ASSERT_EQ(map.find_by_value(0.33f), 10);
  ASSERT_EQ(map.find_by_key(10), 0.33f);
}

TEST(ecmaptests, remove) {
  ECMap<10, int, float> map;
  map.assign(1, 0.1f);
  map.assign(2, 0.2f);
  map.assign(3, 0.3f);
  map.assign(4, 0.4f);
  map.assign(5, 0.5f);
  map.remove_by_key(1);
  ASSERT_THROW(map.find_by_key(1), std::runtime_error);
  ASSERT_THROW(map.find_by_value(0.1f), std::runtime_error);
  map.remove_by_value(0.4f);
  ASSERT_THROW(map.find_by_key(4), std::runtime_error);
  ASSERT_THROW(map.find_by_value(0.4f), std::runtime_error);
}
