#include <entitysubsystem.h>
#include <gtest/gtest.h>

TEST(entitysubsystem_tests, create_entity) {
  EntitySubsystem entity_subsystem;
  auto entity = entity_subsystem.create_entity();
  EXPECT_EQ(entity, 0);
  auto other_entity = entity_subsystem.create_entity();
  EXPECT_EQ(other_entity, 1);
}

TEST(entitysubsystem_tests, create_entity_sig) {
  EntitySubsystem entity_subsystem;
  signature_t sig{0xA};
  auto entity_id = entity_subsystem.create_entity(sig);
  auto signature = entity_subsystem.get_signature(entity_id);
  ASSERT_EQ(sig, signature);
}

TEST(entitysubsystem_tests, destroy_entity) {
  EntitySubsystem entity_subsystem;
  signature_t sig{0xA};
  auto entity_id = entity_subsystem.create_entity(sig);
  auto signature = entity_subsystem.get_signature(entity_id);
  ASSERT_EQ(sig, signature);
  entity_subsystem.destroy_entity(entity_id);
  ASSERT_DEATH(signature = entity_subsystem.get_signature(entity_id),
               "Assertion `entity < m_num_entities' failed");
}

TEST(entitysubsystem_tests, set_signature) {
  EntitySubsystem entity_subsystem;
  auto entity_id = entity_subsystem.create_entity();
  signature_t sig{0xF};
  entity_subsystem.set_signature(entity_id, sig);
  auto signature = entity_subsystem.get_signature(entity_id);
  ASSERT_EQ(sig, signature);
}
