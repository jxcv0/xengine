#include <gtest/gtest.h>

#include <entitysubsystem.h>
#include <componentarray.h>

EntitySubsystem ess;

struct MockComponent {
  float m_x;
  float m_y;
};

TEST(componentarray_tests, add_component) {
  ComponentArray<MockComponent> carr;
  auto entity = ess.create_entity();
  auto component = carr.add(entity, {1.0f, 1.43f});
}

