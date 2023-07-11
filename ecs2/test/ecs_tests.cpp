#include <gtest/gtest.h>
#include "ecs.h"

struct position
{
    float xyz[3];
};

TEST (ecs_tests, create_assign_remove)
{
    xen::ecs ecs;
    auto e1 = ecs.create_entity();
    ASSERT_THROW (ecs.has_components<position>(e1), std::runtime_error);
    ecs.create_component<position>();
    ASSERT_FALSE (ecs.has_components<position>(e1));
    ecs.add_component<position>(e1);
    ASSERT_TRUE (ecs.has_components<position>(e1));
}

TEST (ecs_tests, query)
{

}

