#include <gtest/gtest.h>

#include "ecs.hpp"

struct position {
  float x;
  float y;
};

TEST(ecs_tests, get_component_invalid_id) {}