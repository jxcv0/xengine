#include <gtest/gtest.h>

#include <cassert>
#include <cstdint>
#include <stdexcept>

#include "blockallocator.h"

TEST(blockallocatortests, allocate) {
  BlockAllocator<int> allocator(10);
  auto ptr = allocator.allocate(1);
  ASSERT_NE(ptr, nullptr);
}

TEST(blockallocatortests, allocate_throw) {
  BlockAllocator<float> allocator(1);
  auto f1 = allocator.allocate();
  ASSERT_NE(f1, nullptr);
  ASSERT_THROW(auto f2 = allocator.allocate(), std::runtime_error);
}
