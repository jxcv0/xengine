#include <gtest/gtest.h>

#include "chunk.hpp"

TEST(chunk_tests, dctor) { xen::chunk<9> chunk(42, 0, 0.0f, '0'); }

TEST(chunk_tests, get_by_type_index)
{
  xen::chunk<9> chunk(42, 0, 0.0f, '0');
  int& i = *reinterpret_cast<int*>(
      chunk.get_by_type_index(std::type_index(typeid(int))));
  ASSERT_EQ(i, 0);
  i = 10001;
  int j = *reinterpret_cast<int*>(
      chunk.get_by_type_index(std::type_index(typeid(int))));
  ASSERT_EQ(j, 10001);
  float f = *reinterpret_cast<float*>(
      chunk.get_by_type_index(std::type_index(typeid(float))));
  ASSERT_FLOAT_EQ(f, 0.0f);
  char c = *reinterpret_cast<char*>(
      chunk.get_by_type_index(std::type_index(typeid(char))));
  ASSERT_EQ(c, '0');
}

TEST(chunk_tests, get_component)
{
  xen::chunk<9> chunk(42, 0, 0.0f, '0');
  int& i = chunk.get_component<int>();
  ASSERT_EQ(i, 0);
  i = 1000000001;
  int j = chunk.get_component<int>();
  ASSERT_EQ(j, 1000000001);
  ASSERT_FLOAT_EQ(chunk.get_component<float>(), 0.0f);
  ASSERT_EQ(chunk.get_component<char>(), '0');
}