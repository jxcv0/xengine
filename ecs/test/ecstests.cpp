#include <gtest/gtest.h>

#include "archetypearray.h"
#include "entityarray.h"
#include "gtest/gtest.h"
#include "threadpool.h"

ThreadPool tp(4);

struct A {
  static const int id = 1;
  int i = 0;
};

TEST(ecstests, add) {
  A a;
  a.i = 42;
  EntityArray<8> entities;
  ArchetypeArray<A> archetypes;

  auto e = entities.create_entity();
  entities.add_components(e, archetypes.id());
  archetypes.add_entity(e);
  archetypes.set_component(e, a);

  ASSERT_EQ(archetypes.get_component<A>(e)->i, 42);
  ASSERT_TRUE(entities.has_component(e, 1));
}

struct B {
  static const int id = 2;
  int i = 13;
};

struct C {
  static const int id = 2;
  int i = 120;
};

class Task_SumInA {
 public:
  template <typename... Components>
  Task_SumInA(ArchetypeArray<Components...>& archetype) {}

  virtual ~Task_SumInA() {}

 private:
};

TEST(ecstests, threadpool) {
  EntityArray<8> entities;
  ArchetypeArray<A> archetypes;

  auto e = entities.create_entity();
  entities.add_components(e, archetypes.id());
  archetypes.add_entity(e);
}
