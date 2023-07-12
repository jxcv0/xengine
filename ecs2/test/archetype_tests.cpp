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
