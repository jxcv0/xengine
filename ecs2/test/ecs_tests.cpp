#include <gtest/gtest.h>

#include "ecs.hpp"

struct position
{
  float x;
  float y;
};

TEST(ecs_tests, get_component_invalid_id)
{
  xen::ecs ecs;
  ASSERT_THROW(ecs.get_component<position>(10), std::runtime_error);
}

TEST(ecs_tests, get_component_new_archetype)
{
  xen::ecs ecs;
  auto e1 = ecs.create_entity();
  auto& pos = ecs.get_component<position>(e1);
  ASSERT_EQ(ecs.num_archetypes(), 1);
  (void)pos;
}
