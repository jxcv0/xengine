#include <gtest/gtest.h>

#include <cmath>

#include "componentarray.h"
#include "constants.h"

struct Component {
  int m_id = 0;
  float m_constant = 0;
};

TEST(componentarraytests, assign) {
  ComponentArray<Component> arr;
  ASSERT_EQ(arr.assign(1), 0);
  ASSERT_EQ(arr.assign(MAX_ENTITIES + 1), -1);
  ASSERT_EQ(arr.assign(1), -1);
}

TEST(componentarraytests, erase_and_get) {
  ComponentArray<Component> arr;

  ASSERT_EQ(arr.assign(1), 0);
  ASSERT_EQ(arr.assign(2), 0);
  ASSERT_TRUE(arr.get(1) != nullptr);
  ASSERT_TRUE(arr.get(2) != nullptr);

  arr.erase(1);
  ASSERT_TRUE(arr.get(1) == nullptr);
  ASSERT_TRUE(arr.get(2) != nullptr);

  arr.erase(2);
  ASSERT_TRUE(arr.get(2) == nullptr);
}

TEST(componentarraytests, set) {
  ComponentArray<Component> arr;
  arr.assign(42);
  Component *c = arr.get(42);
  ASSERT_EQ(c->m_id, 0);
  ASSERT_FLOAT_EQ(c->m_constant, 0);
  Component new_component{34, 666.666};
  ASSERT_EQ(arr.set(42, std::move(new_component)), 0);
  c = arr.get(42);
  ASSERT_EQ(c->m_id, 34);
  ASSERT_FLOAT_EQ(c->m_constant, 666.666);
  arr.erase(42);
  ASSERT_EQ(arr.set(42, std::move(new_component)), -1);
}
