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

TEST (archetype_base_tests, add_entity)
{
  xen::archetype_base *arch = new xen::archetype<C1, C2, C3>;
  ASSERT_FALSE (arch->has_entity (10));
  arch->add_entity (10);
  ASSERT_TRUE (arch->has_entity (10));
}

TEST (archetype_base_tests, has_type)
{
  xen::archetype_base *arch = new xen::archetype<C1, C2, C3>;
  bool res = arch->has_type (typeid (C1).hash_code ());
  ASSERT_TRUE (res);
  res = arch->has_type (typeid (C2).hash_code ());
  ASSERT_TRUE (res);
  res = arch->has_type (typeid (C3).hash_code ());
  ASSERT_TRUE (res);
  res = arch->has_type (typeid (C4).hash_code ());
  ASSERT_FALSE (res);
}

TEST (archetype_base_tests, type_count)
{
  xen::archetype_base *one = new xen::archetype<C1, C2, C3>;
  xen::archetype_base *two = new xen::archetype<C1, C2, C3, C4>;
  xen::archetype_base *three = new xen::archetype<C1, C2>;
  ASSERT_EQ (one->type_count (), 3);
  ASSERT_EQ (two->type_count (), 4);
  ASSERT_EQ (three->type_count (), 2);
}

TEST (archetype_base_tests, get_type)
{
  xen::archetype<C1, C2> arch;
  xen::archetype_base *arch_base = &arch;
  arch_base->add_entity (42);
  C1 *c
      = static_cast<C1 *> (arch_base->get_type (42, typeid (C1).hash_code ()));
  c->i = -10;
  C1 res = arch.get_component<C1> (42);
  ASSERT_EQ (res.i, -10);

  ASSERT_EQ (arch_base->get_type (42, typeid (C4).hash_code ()), nullptr);
}

TEST (archetype_base_tests, get_type_templated)
{
  xen::archetype<C1, C2> arch;
  xen::archetype_base *arch_base = &arch;
  arch_base->add_entity (42);
  C1 *c = arch_base->get_component<C1> (42);
  c->i = -10;
  C1 &res = arch.get_component<C1> (42);
  ASSERT_EQ (res.i, -10);

  ASSERT_EQ (arch_base->get_component<C4> (42), nullptr);
}

TEST (archetype_base_tests, get_at_index)
{
  auto arch = new xen::archetype<C1, C2>;

  arch->add_entity (0);
  arch->add_entity (2);
  arch->add_entity (42);

  C1 &tmp_a = arch->get_component<C1> (0);
  C1 &tmp_b = arch->get_component<C1> (2);
  C1 &tmp_c = arch->get_component<C1> (42);

  tmp_a.i = 10;
  tmp_b.i = 11;
  tmp_c.i = 12;

  C1 *a = arch->get_component_by_index<C1> (0);
  C1 *b = arch->get_component_by_index<C1> (1);
  C1 *c = arch->get_component_by_index<C1> (2);

  ASSERT_EQ (a->i, tmp_a.i);
  ASSERT_EQ (b->i, tmp_b.i);
  ASSERT_EQ (c->i, tmp_c.i);
}
