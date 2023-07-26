#include <gtest/gtest.h>

#include "ecs.hpp"

TEST(archetype_tests, has_types) {
  xen::archetype_interface *arch = new xen::archetype<int, float, char>;
  bool b = arch->has_components<int, float>();
  ASSERT_TRUE(b);
  b = arch->has_components<int, char>();
  ASSERT_TRUE(b);
  b = arch->has_components<float, char>();
  ASSERT_TRUE(b);
  b = arch->has_components<double, char>();
  ASSERT_FALSE(b);
}

TEST(archetype_tests, num_types) {
  xen::archetype_interface *arch = new xen::archetype<int, float, char>;
  ASSERT_EQ(arch->num_component_types(), 3);
}

TEST(archetype_tests, entity_not_found) {
  xen::archetype_interface *arch = new xen::archetype<int, float, char>;
  ASSERT_THROW(arch->get_component<int>(42), std::out_of_range);
}

TEST(archetype_tests, get_component) {
  xen::archetype_interface *arch = new xen::archetype<int, float, char>;
  arch->add_entity(42, 1, 2.0f, '3');
  ASSERT_EQ(arch->get_component<int>(42), 1);
  ASSERT_FLOAT_EQ(arch->get_component<float>(42), 2.0f);
  ASSERT_EQ(arch->get_component<char>(42), '3');
}

TEST(ecs_tests, create_entity) {
  struct c {
    int i = 0;
  };
  xen::ecs ecs;
  xen::eid_t e1 = ecs.create_entity<int, float, struct c>(10, 1.0f, {111});
  auto arch = ecs.get_archetype<int, float, struct c>();
  ASSERT_EQ(arch->get_component<int>(e1), 10);
  ASSERT_FLOAT_EQ(arch->get_component<float>(e1), 1.0f);
  ASSERT_EQ(arch->get_component<struct c>(e1).i, 111);
}
