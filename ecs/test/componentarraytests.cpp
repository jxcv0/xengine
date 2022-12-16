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
  auto addr = reinterpret_cast<uintptr_t>(p);
  auto b = reinterpret_cast<B *>(addr + sizeof(A));
  ASSERT_EQ(b->i, -42);
  std::free(p);
}

TEST(archetypetest, get) {
  auto size = sizeof(A) + sizeof(B) + sizeof(C);
  void *p = std::malloc(size);
  Archetype<A, B, C> arch(p);
  auto a = get_component<A>(arch);
  auto b = get_component<B>(arch);
  auto c = get_component<C>(arch);
  ASSERT_EQ(a->i, 42);
  ASSERT_EQ(b->i, -42);
  ASSERT_EQ(c->i, 10);
  std::free(p);
}
