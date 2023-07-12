#include "ecs.h"
#include <gtest/gtest.h>

struct position
{
  float xyz[3];
};

TEST (ecs_tests, create_assign_remove)
{
  xen::ecs ecs;
  auto e1 = ecs.create_entity ();
  (void) e1;
}

TEST (ecs_tests, query) {}
