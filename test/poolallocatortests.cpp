#include <gtest/gtest.h>

#include <cassert>
#include <cstdint>
#include <stdexcept>

#include "poolallocator.h"

TEST(poolallocatortests, allocate) {
  PoolAllocator<int, 1> allocator;
  auto ptr = allocator.allocate();
  ASSERT_NE(ptr, nullptr);
}

TEST(poolallocatortests, allocate_throw) {
  PoolAllocator<float, 1> allocator;
  auto f1 = allocator.allocate();
  ASSERT_NE(f1, nullptr);
  ASSERT_THROW(auto f2 = allocator.allocate(), std::runtime_error);
}
