#include <gtest/gtest.h>

#include "entityarray.h"

#define COMPONENTARRAY_TEST
#include "componentarray.h"

#include <stdexcept>

struct MockComponent {
  int m_id;
};

TEST(componentarraytests, assign) {
  EntityArray<10> es;
  auto e = es.create_entity();
  MockComponent m;
  m.m_id = 42;

  ComponentArray<MockComponent> ca;
  ASSERT_EQ(ca.m_num_components, 0);
  ca.assign(e, m);
  ASSERT_EQ(ca.m_num_components, 1);
  ASSERT_EQ(ca.m_map[0].m_handle, e);
  ASSERT_EQ(ca.m_map[0].m_index, 0);
  ASSERT_EQ(m.m_id, ca.get_component(e)->m_id);
}

TEST(componentarraytests, erase_entity) {
  EntityArray<10> es;
  auto e = es.create_entity();

  float f = 0.33435f;
  ComponentArray<float> a;
  a.assign(e, f);
  ASSERT_FLOAT_EQ(*a.get_component(e), 0.33435f);
  a.erase_entity(e);
  ASSERT_EQ(a.get_component(e), nullptr);
}
