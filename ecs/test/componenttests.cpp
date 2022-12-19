#include <gtest/gtest.h>

#define COMPONENT_GTEST
#include "component.h"

struct A {
  float x = 1.3;
  float y = 42;
  float z = 4.00098;
};

TEST(componenttests, constructor) {
  A a;
  Component<A> component(&a);
  ASSERT_EQ(reinterpret_cast<A *>(component.m_data)->x, a.x);
  ASSERT_EQ(reinterpret_cast<A *>(component.m_data)->y, a.y);
  ASSERT_EQ(reinterpret_cast<A *>(component.m_data)->z, a.z);
}
