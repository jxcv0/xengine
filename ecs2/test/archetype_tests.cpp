#include <gtest/gtest.h>

#include "archetype.hpp"

TEST(chunk_tests, create)
{
  xen::chunk<int, float, char> chunk(0);
  int& i = chunk.get_component<int>();
  i = 10;
  ASSERT_EQ(chunk.get_component<int>(), i);
}

TEST(archetype_tests, create)
{
  xen::archetype arch = xen::archetype::create<int, float, char>();
  ASSERT_EQ(arch.chunk_size(), 9);
}
