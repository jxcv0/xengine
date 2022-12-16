#include <gtest/gtest.h>

#include "archetypearray.h"
#include "entityarray.h"

struct A {
  static const int id = 1;
  int i = 0;
};

TEST(ecstests, sd) {
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
