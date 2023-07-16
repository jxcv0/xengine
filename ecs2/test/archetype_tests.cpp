#include <gtest/gtest.h>

#include "archetype.hpp"

TEST(archetype_tests, create)
{
  xen::archetype arch = xen::archetype::create<int, float, char>();
  ASSERT_EQ(arch.chunk_size(), 9);
}
