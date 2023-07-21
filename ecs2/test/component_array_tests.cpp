#include <gtest/gtest.h>

#include "component_array.h"

struct position {
  float vec[3];
};

TEST(component_array_tests, assign_and_remove) {
  xen::component_array<position> posarr;
  posarr.assign(10);
  posarr.assign(11);
  ASSERT_NO_THROW(posarr.get(10));
  posarr.remove(10);
  ASSERT_THROW(posarr.get(10), std::runtime_error);
  ASSERT_NO_THROW(posarr.get(11));
}

TEST(component_array_tests, get_and_set_component) {
  xen::component_array<position> posarr;
  posarr.assign(10);
  position pos = posarr.get(10);
  pos.vec[0] = -20.0f;
  posarr.set(10, pos);
  position p = posarr.get(10);
  ASSERT_FLOAT_EQ(p.vec[0], -20.0f);
}
