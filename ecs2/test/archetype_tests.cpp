#include <gtest/gtest.h>

#include "archetype.hpp"

struct C1
{
  int i;
};

struct C2
{
  int i;
};

struct C3
{
  int i;
};

TEST(archetype_tests, create)
{
  C1 c1;
  c1.i = 1;

  C2 c2;
  c2.i = 2;

  C3 c3;
  c3.i = 3;

  xen::archetype arch(42, c1, c2, c3);
  ASSERT_EQ(arch.get_component<C1>(42).i, 1);
  ASSERT_EQ(arch.get_component<C2>(42).i, 2);
  ASSERT_EQ(arch.get_component<C3>(42).i, 3);
}

TEST(archetype_tests, get_component)
{
  xen::archetype arch(42, C1{ 1 }, C2{ 2 }, C3{ 3 });
  C1& c = arch.get_component<C1>(42);
  ASSERT_EQ(c.i, 1);
  c.i = 10001;
  ASSERT_EQ(arch.get_component<C1>(42).i, 10001);
  ASSERT_EQ(arch.get_component<C2>(42).i, 2);
  ASSERT_EQ(arch.get_component<C3>(42).i, 3);
}

TEST(archetype_tests, add_remove_entity)
{
  xen::archetype arch(42, C1{ 1 }, C2{ 2 }, C3{ 3 });
  ASSERT_THROW(arch.get_component<C3>(666), std::out_of_range);
  arch.add_entity(666, C1{ 11 }, C2{ 12 }, C3{ 13 });
  ASSERT_EQ(arch.get_component<C1>(666).i, 11);
  ASSERT_EQ(arch.get_component<C2>(666).i, 12);
  ASSERT_EQ(arch.get_component<C3>(666).i, 13);
  arch.remove_entity(42);
  ASSERT_THROW(arch.get_component<C3>(42), std::out_of_range);
}

TEST(archetype_tests, has_entity)
{
  xen::archetype arch(42, C1{ 1 }, C2{ 2 }, C3{ 3 });
  ASSERT_TRUE(arch.has_entity(42));
  ASSERT_FALSE(arch.has_entity(666));
  arch.add_entity(666, C1{ 11 }, C2{ 12 }, C3{ 13 });
  ASSERT_TRUE(arch.has_entity(42));
  ASSERT_TRUE(arch.has_entity(666));
  arch.remove_entity(42);
  ASSERT_FALSE(arch.has_entity(42));
  ASSERT_TRUE(arch.has_entity(666));
}

TEST(archetype_tests, has_component)
{
  xen::archetype arch(42, C1{ 1 }, C2{ 2 }, C3{ 3 });
  ASSERT_TRUE(arch.has_component<C1>());
  ASSERT_FALSE(arch.has_component<double>());
}