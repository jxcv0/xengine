#include <gtest/gtest.h>
#include "ecs.h"

struct position
{
    float xyz[3];
};

TEST (ecs_tests, create_and_assign)
{
    xen::ecs ecs;
    auto e1 = ecs.create_entity();
    ecs.create_component<position>();
    ecs.add_component<position>(e1);
}
