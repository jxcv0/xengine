#include <gtest/gtest.h>

#include <entitysubsystem.h>

TEST(placeholdersuite, placeholdertest) {
  EntitySubsystem entity_subsystem;
  auto entity = entity_subsystem.create_entity();
  EXPECT_EQ(entity, 0);
}
