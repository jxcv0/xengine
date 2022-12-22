#include <gtest/gtest.h>

#include "componentarray.h"
#include "constants.h"
#include "ecs.h"

struct A {
  static const int component_id = (1 << 1);
  int i = 0;
};

struct B {
  static const int component_id = (1 << 2);
  int i = 13;
};

struct C {
  static const int component_id = (1 << 3);
  int i = 120;
};

TEST(ecstests, create_entity) {
  ECS ecs;
  entity_id id = ecs.create_entity();
  ASSERT_EQ(id, 1);
}

TEST(ecstests, register_component) {
  ECS ecs;
  entity_id id = ecs.create_entity();
  ecs.register_component<A>(id);
  ComponentArray<A> *arr = ecs.get_component_array<A>();
  ASSERT_TRUE(arr != nullptr);
}
