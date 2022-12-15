#include <gtest/gtest.h>

#include "entityarray.h"

TEST(entitysubsystemtests, create_entity) {
  EntityArray es;
  ASSERT_EQ(es.count(), 0);
  auto e1 = es.create_entity();
  ASSERT_EQ(es.count(), 1);
  ASSERT_EQ(e1, 0);
  auto e2 = es.create_entity();
  ASSERT_EQ(es.count(), 2);
  ASSERT_EQ(e2, 1);
}

TEST(entitysubsystemtests, signature_initialized_to_zero) {
  EntityArray<10> es;
  auto e = es.create_entity();
  ASSERT_EQ(e, 0);
  ASSERT_FALSE(es.has_component(e, 0));  // 0 & 0
}

TEST(entitysubsystemtests, create_entity_throw) {
  EntityArray<3> es;
  auto e1 = es.create_entity();
  ASSERT_EQ(e1, 0);
  auto e2 = es.create_entity();
  ASSERT_EQ(e2, 1);
  auto e3 = es.create_entity();
  ASSERT_EQ(e3, 2);
  ASSERT_EQ(es.create_entity(), -1);
  ASSERT_EQ(e3, 2);
}

TEST(entitysubsystem_tests, set_signature) {
  EntityArray<10> es;
  auto e = es.create_entity();
  es.add_component(e, 10);
  ASSERT_TRUE(es.has_component(e, 10));
}

TEST(entitysubsystem_tests, amend_signature) {
  EntityArray<10> es;
  auto e = es.create_entity();
  es.add_component(e, 0b1);
  ASSERT_TRUE(es.has_component(e, 1));  // test for all
  es.add_component(e, 0b10);
  ASSERT_TRUE(es.has_component(e, 0b10));
  ASSERT_TRUE(es.has_component(e, 0b1));
}

TEST(entitysubsystem_tests, erase_entity) {
  EntityArray<10> es;
  auto e = es.create_entity();
  es.add_component(e, 0b100);
  ASSERT_TRUE(es.has_component(e, 4));
  ASSERT_EQ(es.count(), 1);
  es.erase_entity(e);
  ASSERT_FALSE(es.has_component(e, 0));
  ASSERT_EQ(es.count(), 0);
}
