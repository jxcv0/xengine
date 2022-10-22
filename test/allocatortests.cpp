#include <allocator.h>
#include <gtest/gtest.h>

#include <cassert>
#include <cstdint>
#include <memory>
#include <new>
#include <stdexcept>

TEST(allocatortests, allocate) {
  Allocator<int> allocator;
  auto ptr = allocator.allocate(1);
  ASSERT_NE(ptr, nullptr);
  ASSERT_EQ(allocator.capacity(), 1);
}

TEST(allocatortests, allocate_array) {
  Allocator<std::string> str_alloc;
  std::string *strs = str_alloc.allocate(4);
  strs[0] = "1";
  strs[1] = "2";
  strs[2] = "3";
  strs[3] = "4";
  ASSERT_EQ(strs[0], "1");
  ASSERT_EQ(strs[3], "4");
}

TEST(allocatortests, stdlib_allocator) {
  std::vector<std::string, Allocator<std::string>> vec;
  for (int i = 0; i < 5; i++) {
    vec.push_back(std::to_string(i));
  }
  ASSERT_EQ(vec[0], "0");
  ASSERT_EQ(vec[1], "1");
  ASSERT_EQ(vec[2], "2");
  ASSERT_EQ(vec[3], "3");
  ASSERT_EQ(vec[4], "4");
}
