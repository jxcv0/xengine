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

struct D {
  const static int id = 4;
  int i = 0;
};

TEST(archetypetests, size_and_count) {
  Archetype<A, B, C> a(1);
  ASSERT_EQ(a.id(), 1 | 2 | 3);
  ASSERT_EQ(decltype(a)::id(), 1 | 2 | 3);
  ASSERT_EQ(a.count(), 3);
  ASSERT_EQ(a.size() + sizeof(int), sizeof(Archetype<A, B, C>));
}

TEST(archetypetests, get_component) {
  Archetype<A, B, C> arch(1);
  auto &a = arch.get_component<A>();
  auto &b = arch.get_component<B>();
  auto &c = arch.get_component<C>();
  ASSERT_EQ(a.i, 42);
  a.i = 10;
  ASSERT_EQ(arch.get_component<A>().i, 10);
  ASSERT_EQ(b.i, -42);
  ASSERT_EQ(c.i, 10);
}

TEST(archetypetests, set_component) {
  A a;
  a.i = 15;
  Archetype<A> arch(1);
  arch.set_component(a);
  ASSERT_EQ(arch.get_component<A>().i, 15);
  ASSERT_EQ(a.i, 15);
}

TEST(archetypearraytests, add_erase_entity) {
  int entityid = 14;
  ArchetypeArray<A, B, C> a(10);
  a.add_entity(entityid);
  auto &c = a.get_component<A>(entityid);
  ASSERT_EQ(c.i, 42);
  a.remove_entity(entityid);
  // c = a.get_component<A>(12);
  // ASSERT_EQ(c, nullptr);
}

TEST(archetypearraytests, set_component) {
  A component;
  component.i = 15;
  ArchetypeArray<A, B, C> a(10);
  a.add_entity(12);
  a.set_component(12, component);
  ASSERT_EQ(component.i, 15);
  ASSERT_EQ(a.get_component<A>(12).i, 15);
}

TEST(archetypearraytests, iterator) {
  ArchetypeArray<A, B, C> a(10);
  for (int i = 1; i < 11; i++) {
    A component;
    component.i = i * 2;
    a.add_entity(i);
    a.set_component(i, component);
  }

  ASSERT_EQ(a.size(), 10);

  int i = 1;
  for (auto iter = a.begin<A>(); iter != a.end<A>(); iter++) {
    std::cout << "here\n";
    // ASSERT_EQ(c->i, i * 2);
    i++;
  }
}
