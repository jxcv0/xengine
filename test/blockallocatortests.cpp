#include <gtest/gtest.h>

#include "blockallocator.h"

struct Foo {
  int bar;
};

TEST(blockallocatortests, allocate) {
  BlockAllocator<Foo> allocator(10);
  auto foo = allocator.allocate(1);
  ASSERT_TRUE(foo != nullptr);
}
