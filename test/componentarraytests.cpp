#include <gtest/gtest.h>

#include <entity.h>
#include <component.h>

struct MockComponent {
  int m_id;
};

TEST(componentarraytests, assign) {
  EntitySystem<10> es;
  auto e = es.create_entity();
  MockComponent m = {42};

  ComponentArray<MockComponent> a;
  a.assign(e, m);
  ASSERT_EQ(m.m_id, a.get_component(e).m_id);
}
