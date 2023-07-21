#include <gtest/gtest.h>

#include "ecs.hpp"

struct position {
  float x;
  float y;
};

TEST(ecs_tests, get_component_invalid_id) {
  xen::ecs ecs;
  ASSERT_THROW(ecs.add_component<position>(10), std::invalid_argument);
}

TEST(ecs_tests, has_components) {
  xen::ecs ecs;
  xen::eid_t e1 = ecs.create_entity();
  ecs.add_component<position>(e1);
}