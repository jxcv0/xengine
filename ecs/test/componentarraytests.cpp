#include <component.h>
#include <entity.h>
#include <gtest/gtest.h>

#include <stdexcept>

struct MockComponent {
  int m_id;
};

TEST(componentarraytests, assign) {
  EntitySystem<10> es;
  auto e = es.create_entity();
  MockComponent m;
  m.m_id = 42;

  ComponentArray<MockComponent> a;
  a.assign(e, m);
  ASSERT_EQ(m.m_id, a.get_component(e)->m_id);
}

TEST(componentarraytests, erase_entity) {
  EntitySystem<10> es;
  auto e = es.create_entity();

  float f = 0.33435f;
  ComponentArray<float> a;
  a.assign(e, f);
  ASSERT_FLOAT_EQ(*a.get_component(e), 0.33435f);
  a.erase_entity(e);
  ASSERT_EQ(a.get_component(e), nullptr);
}
