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
  ASSERT_EQ(a->i, 42);
  a->i = 10;
  ASSERT_EQ(arch.get_component<A>()->i, 10);
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

TEST(archetypearraytests, add_erase_entity) {
  ArchetypeArray<A, B, C> a;
  a.add_entity(12);
  auto c = a.get_component<A>(12);
  c = a.get_component<A>(12);
  ASSERT_EQ(c->i, 42);
  a.remove_entity(12);
  // c = a.get_component<A>(12);
  // ASSERT_EQ(c, nullptr);
}

TEST(archetypearraytests, set_component) {
  A component;
  component.i = 15;
  ArchetypeArray<A, B, C> a;
  a.add_entity(12);
  a.set_component(12, component);
  ASSERT_EQ(component.i, 15);
  ASSERT_EQ(a.get_component<A>(12)->i, 15);
}
