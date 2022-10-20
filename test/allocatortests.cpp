#include <gtest/gtest.h>
#include <allocator.h>

#include <cassert>
#include <cstdint>
#include <new>
#include <stdexcept>


TEST(allocatortests, allocate) {
  Allocator<int, 1> allocator;
  auto ptr = allocator.allocate(1);
  ASSERT_NE(ptr, nullptr);
}

TEST(allocatortests, allocate_throw) {
  Allocator<float, 1> allocator;
  auto f1 = allocator.allocate(1);
  ASSERT_NE(f1, nullptr);
  ASSERT_THROW(auto f2 = allocator.allocate(1), std::bad_alloc);
}

TEST(allocatortests, allocate_array) {
  Allocator<std::string, 10> str_alloc;
  std::string *strs = str_alloc.allocate(4);
  strs[0] = "1";
  strs[1] = "2";
  strs[2] = "3";
  strs[3] = "4";
  ASSERT_EQ(strs[0], "1");
  ASSERT_EQ(strs[3], "4");
}

TEST(allocatortests, stdlib_allocator) {
  std::vector<int, Allocator<int, 5>> vec;
  for (int i = 0; i < 5; i++) {
    vec.push_back(i);
  }
  ASSERT_EQ(vec[0], 0);
  ASSERT_EQ(vec[1], 1);
  ASSERT_EQ(vec[2], 2);
  ASSERT_EQ(vec[3], 3);
  ASSERT_EQ(vec[4], 4);
  ASSERT_THROW(vec.push_back(5), std::bad_alloc);
}
