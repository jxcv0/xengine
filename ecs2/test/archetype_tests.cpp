#include <gtest/gtest.h>

#include "archetype.hpp"

TEST(chunk_tests, create)
{
  xen::chunk<int, float, char> chunk(0);
  int& i = chunk.get_component<int>();
  i = 10;
  ASSERT_EQ(chunk.get_component<int>(), i);

  char& c = chunk.get_component<char>();
  c = 'Z';
  ASSERT_EQ(chunk.get_component<char>(), 'Z');
  c = 'e';
  ASSERT_EQ(chunk.get_component<char>(), 'e');
}

TEST(archetype_tests, create)
{
  xen::archetype arch = xen::archetype::create<int, float, char>();
  ASSERT_EQ(arch.chunk_size(), 9);
}

TEST(archetype_tests, has_entity)
{
  xen::archetype arch = xen::archetype::create<int>();
  ASSERT_FALSE(arch.has_entity(10));
  arch.add_entity(10);
  ASSERT_TRUE(arch.has_entity(10));
}

TEST(archetype_tests, size)
{
  xen::archetype arch = xen::archetype::create<int>();
  ASSERT_EQ(arch.size(), 0);
  arch.add_entity(10);
  ASSERT_EQ(arch.size(), 1);
}

TEST(archetype_tests, get)
{
  xen::archetype arch = xen::archetype::create<int, double, float>();
  arch.add_entity(42);
  double& d = arch.get<double>(42);
  d = 100.0001f;
  float& f = arch.get<float>(42);
  f = 42.01f;
  ASSERT_FLOAT_EQ(arch.get<float>(42), 42.01f);
  int& i = arch.get<int>(42);
  i = -10;
  ASSERT_EQ(arch.get<int>(42), -10);
  ASSERT_FLOAT_EQ(arch.get<float>(42), 42.01f);
  ASSERT_FLOAT_EQ(arch.get<double>(42), 100.0001f);
}

TEST(archetype_tests, add_entity_with_vals)
{
  struct C1
  {
    int i;
  };

  C1 prev;
  prev.i = 88;

  xen::archetype arch = xen::archetype::create<int, C1>();
  arch.add_entity(42, 69, prev);
  ASSERT_EQ(arch.get<C1>(42).i, 88);
  ASSERT_EQ(arch.get<int>(42), 69);
}

TEST(archetype_tests, throw_on_get)
{
  xen::archetype arch = xen::archetype::create<int, float>();
  arch.add_entity(42);
  ASSERT_THROW(arch.get<char>(44), std::runtime_error);
}
