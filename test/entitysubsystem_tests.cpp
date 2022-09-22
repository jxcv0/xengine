#include <gtest/gtest.h>

#include <entitysubsystem.h>

TEST(placeholdersuite, placeholdertest) {
  auto entity = EntitySubsystem::instance().create_entity();
  EXPECT_EQ(entity, 0);
}
