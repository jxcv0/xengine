#include <gtest/gtest.h>

#include <cstdint>
#include <iterator>
#include <tuple>

#include "archetypearray.h"

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

TEST(archetypetest, size_and_count) {
  auto size = sizeof(A) + sizeof(B) + sizeof(C);
  Archetype<A, B, C> a;
  ASSERT_EQ(a.id(), 1 | 2 | 3);
  ASSERT_EQ(decltype(a)::id(), 1 | 2 | 3);
  ASSERT_EQ(a.count(), 3);
  ASSERT_EQ(a.size(), size);
  ASSERT_EQ(a.size(), sizeof(Archetype<A, B, C>));
}

struct D {
  const static int id = 4;
};

TEST(archetypetest, get_component) {
  Archetype<A, B, C> arch;
  auto a = arch.get_component<A>();
  auto b = arch.get_component<B>();
  auto c = arch.get_component<C>();
  auto d = arch.get_component<D>();
  (void)d;
  ASSERT_EQ(a->i, 42);
  ASSERT_EQ(b->i, -42);
  ASSERT_EQ(c->i, 10);
  ASSERT_EQ(d, nullptr);
}

TEST(archetypetest, set_component) {
  A a;
  a.i = 15;
  Archetype<A> arch;
  arch.set_component(a);
  ASSERT_EQ(arch.get_component<A>()->i, 15);
  ASSERT_EQ(a.i, 15);
}

TEST(archetypearraytests, add) {
  ArchetypeArray<A, B, C> aa;
  ASSERT_EQ(aa.id(), 1 | 2 | 3);
}
