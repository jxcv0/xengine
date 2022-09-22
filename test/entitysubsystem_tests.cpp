#include <gtest/gtest.h>

#include <entitysubsystem.h>

TEST(entitysubsystem_tests, create_entity) {
  EntitySubsystem entity_subsystem;
  auto entity = entity_subsystem.create_entity();
  EXPECT_EQ(entity, 0);
  auto other_entity = entity_subsystem.create_entity();
  EXPECT_EQ(other_entity, 1);
}

TEST(entitysubsystem_tests, create_entity_with_signature) {
  EntitySubsystem entity_subsystem;
  auto entity_id = entity_subsystem.create_entity();
  auto signature = entity_subsystem.get_signature(entity_id);
  //assert
}
