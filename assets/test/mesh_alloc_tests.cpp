#include <gtest/gtest.h>

#include "assets.h"

TEST (mesh_allocator_tests, allocator)
{
  struct mesh_allocator alloc = create_mesh_allocator (10);
  int one = alloc_mesh (&alloc);
  ASSERT_EQ (one, 0);
  ASSERT_EQ (alloc.nmeshes, 1);
  ASSERT_EQ (alloc.nfree, 9);
  int two = alloc_mesh (&alloc);
  ASSERT_EQ (two, 1);
  ASSERT_EQ (alloc.nmeshes, 2);
  ASSERT_EQ (alloc.nfree, 8);
  free_mesh (&alloc, one);
  ASSERT_EQ (alloc.nmeshes, 1);
  ASSERT_EQ (alloc.nfree, 9);
  ASSERT_EQ (alloc.free[8], 0);
  int three = alloc_mesh (&alloc);
  ASSERT_EQ (three, one);
}
