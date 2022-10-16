#include <cassert>
#include <cstdint>
#include <gtest/gtest.h>
#include <stdexcept>

#include "blockallocator.h"

TEST(blockallocatortests, allocate) {
  BlockAllocator<int> allocator(10);
  auto foo = allocator.allocate(1);
  ASSERT_TRUE(foo != nullptr);
}

TEST(blockallocatortests, allocate_throw) {
  BlockAllocator<float> allocator(1);
  auto f1 = allocator.allocate();
  ASSERT_THROW(auto f2 = allocator.allocate(), std::runtime_error);
}

TEST(blockallocatortests, allocate_static) {
  BlockAllocator<int> allocator1(2);
  BlockAllocator<int> allocator2(2);
  auto one = allocator1.allocate();
  auto two = allocator2.allocate();
  ASSERT_THROW(auto three = allocator1.allocate(), std::runtime_error);
}
