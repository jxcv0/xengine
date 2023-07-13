#include <gtest/gtest.h>

#include "archetype.h"

struct C1
{
  int i;
};

struct C2
{
  int j;
};

struct C3
{
  int k;
};

struct C4
{
  int l;
};

TEST (archetype_tests, has_component)
{
  xen::archetype<C1, C2, C3> arch;
  bool res = arch.has_component<C1> ();
  ASSERT_TRUE (res);
  res = arch.has_component<C2> ();
  ASSERT_TRUE (res);
  res = arch.has_component<C3> ();
  ASSERT_TRUE (res);
  res = arch.has_component<C4> ();
  ASSERT_FALSE (res);
}

TEST (archetype_tests, has_components)
{
  xen::archetype<C1, C2, C3> arch;
  bool res = arch.has_components<C1> ();
  ASSERT_TRUE (res);
  res = arch.has_components<C1, C2> ();
  ASSERT_TRUE (res);
  res = arch.has_components<C1, C2, C3> ();
  ASSERT_TRUE (res);
  res = arch.has_components<C1, C2, C3, C4> ();
  ASSERT_FALSE (res);
}

TEST (archetype_tests, register_entity)
{
  std::uint64_t entity = 42;
  xen::archetype<int, float, char> arch;
  arch.register_entity (entity, 10, 20.0f, '3');
  int &i = arch.get_component<int> (entity);
  ASSERT_EQ (i, 10);

  /* non ref just to check */
  float f = arch.get_component<float> (entity);
  ASSERT_FLOAT_EQ (f, 20.0f);

  char &c = arch.get_component<char> (entity);
  ASSERT_EQ (c, '3');
}

TEST (archetype_base_tests, add_entity)
{
  xen::archetype_base *arch = new xen::archetype<C1, C2, C3>;
  ASSERT_FALSE (arch->has_entity(10));
  arch->add_entity(10);
  ASSERT_TRUE (arch->has_entity(10));
}

TEST (archetype_base_tests, has_type)
{
  xen::archetype_base *arch = new xen::archetype<C1, C2, C3>;
  bool res = arch->has_type(typeid(C1).hash_code());
  ASSERT_TRUE (res);
  res = arch->has_type(typeid(C2).hash_code());
  ASSERT_TRUE (res);
  res = arch->has_type(typeid(C3).hash_code());
  ASSERT_TRUE (res);
  res = arch->has_type(typeid(C4).hash_code());
  ASSERT_FALSE (res);
}

TEST (archetype_base_tests, type_count)
{
  xen::archetype_base *one = new xen::archetype<C1, C2, C3>;
  xen::archetype_base *two = new xen::archetype<C1, C2, C3, C4>;
  xen::archetype_base *three = new xen::archetype<C1, C2>;
  ASSERT_EQ (one->type_count(), 3);
  ASSERT_EQ (two->type_count(), 4);
  ASSERT_EQ (three->type_count(), 2);
}


