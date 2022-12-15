#include <gtest/gtest.h>

#include <cstdint>

#include "componentarray.h"

struct A {
  const static int id = 1;
  A() { i = 42; }
  int i = 0;
};

struct B {
  const static int id = 2;
};

struct C {
  const static int id = 3;
};

TEST(archetypetest, size) {
  auto size = sizeof(A) + sizeof(B) + sizeof(C);
  void *p = std::malloc(size);
  Archetype<A, B, C> a(p);
  ASSERT_EQ(a.id(), 1 | 2 | 3);
  ASSERT_EQ(a.count(), 3);
  ASSERT_EQ(a.size(), size);
  std::free(p);
}

TEST(archetypetest, default_construction) {
  auto size = sizeof(A) + sizeof(B) + sizeof(C);
  void *p = std::malloc(size);
  Archetype<A, B, C> a(p);
  ASSERT_EQ(static_cast<A *>(p)->i, 42);
  std::free(p);
}
