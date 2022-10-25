#include <entity.h>
#include <gtest/gtest.h>

#include <stdexcept>

#include "entitysubsystem.h"

TEST(entitysubsystemtests, create_entity) {
  EntitySystem<10> es;
  auto e1 = es.create_entity();
  ASSERT_EQ(e1, 0);
  auto e2 = es.create_entity();
  ASSERT_EQ(e2, 1);
}

TEST(entitysubsystemtests, signature_initialized_to_zero) {
  EntitySystem<10> es;
  auto e = es.create_entity();
  ASSERT_EQ(e, 0);
  auto s = es.get_signature(e);
  ASSERT_EQ(s, 0);
}

TEST(entitysubsystemtests, create_entity_throw) {
  EntitySystem<3> es;
  auto e1 = es.create_entity();
  ASSERT_EQ(e1, 0);
  auto e2 = es.create_entity();
  ASSERT_EQ(e2, 1);
  auto e3 = es.create_entity();
  ASSERT_EQ(e3, 2);
  ASSERT_THROW(auto e3 = es.create_entity(), std::runtime_error);
}

TEST(entitysubsystem_tests, set_signature) {
  EntitySystem<10> es;
  auto e = es.create_entity();
  ASSERT_EQ(es.get_signature(e), 0);
  es.set_signature(e, 10);
  auto s = es.get_signature(e);
  ASSERT_EQ(s, 10);
}

TEST(entitysubsystem_tests, amend_signature) {
  EntitySystem<10> es;
  auto e = es.create_entity();
  ASSERT_EQ(es.get_signature(e), 0);
  es.set_signature(e, 0x1);
  auto s = es.get_signature(e);
  ASSERT_EQ(s, 1);
  es.amend_signature(e, 0x2);
  s = es.get_signature(e);
  ASSERT_EQ(s, (0b10 & s));
  ASSERT_EQ(s, (0b1 & s));
}

TEST(entitysubsystem_tests, erase_entity) {
  EntitySystem<10> es;
  auto e = es.create_entity();
  es.set_signature(e, 0b100);
  ASSERT_EQ(es.get_signature(e), 4);
  es.erase_entity(e);
  ASSERT_EQ(es.get_signature(e), 0);
}
