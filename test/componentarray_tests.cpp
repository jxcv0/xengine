#include <asm-generic/errno.h>
#include <componentarray.h>
#include <entitysubsystem.h>
#include <gtest/gtest.h>

#include "gtest/gtest.h"

EntitySubsystem ess;
eid_t entity_id;

struct MockComponent {
  float m_x;
  float m_y;
};

ComponentArray<MockComponent> carr;
cid_t component_id;

TEST(componentarray_tests, add) {
  entity_id = ess.create_entity();
  component_id = carr.add(entity_id, {1.0f, 1.43f});
  ASSERT_EQ(component_id, 0);
  auto component = carr.get(entity_id);
  ASSERT_FLOAT_EQ(component.m_x, 1.0f);
  ASSERT_FLOAT_EQ(component.m_y, 1.43f);
}

TEST(componentarray_tests, reset) {
  carr.reset(entity_id);
  ASSERT_DEATH(carr.get(entity_id),
               "Assertion `it != m_map\\.end\\(\\)' failed");
}

TEST(componentarray_tests, notify_destroyed) {
  component_id = carr.add(entity_id, {1.0f, 1.43f});
  ASSERT_EQ(component_id, 0);
  auto component = carr.get(entity_id);
  ASSERT_FLOAT_EQ(component.m_x, 1.0f);
  ASSERT_FLOAT_EQ(component.m_y, 1.43f);
  carr.notify_destroyed(entity_id);
  ASSERT_DEATH(carr.get(entity_id),
               "Assertion `it != m_map\\.end\\(\\)' failed");
}
