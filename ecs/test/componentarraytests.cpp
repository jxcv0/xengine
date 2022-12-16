#include <gtest/gtest.h>

#include <cstdint>
#include <iterator>
#include <tuple>

#include "componentarray.h"

struct A {
  const static int id = 1;
  A() { i = 42; }
  int i = 0;
};

struct B {
  const static int id = 2;
  B() { i = -42; }
  int i = 0;
};

struct C {
  const static int id = 3;
  int i = 10;
};

TEST(archetypetest, size) {
  auto size = sizeof(A) + sizeof(B) + sizeof(C);
  Archetype<A, B, C> a;
  ASSERT_EQ(a.id(), 1 | 2 | 3);
  ASSERT_EQ(a.count(), 3);
  ASSERT_EQ(a.size(), size);
  ASSERT_EQ(a.size() + sizeof(int), sizeof(Archetype<A, B, C>));
}

TEST(archetypetest, get) {
  Archetype<A, B, C> arch;
  auto a = get_component<A>(arch);
  auto b = get_component<B>(arch);
  auto c = get_component<C>(arch);
  ASSERT_EQ(a->i, 42);
  ASSERT_EQ(b->i, -42);
  ASSERT_EQ(c->i, 10);
}
