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
