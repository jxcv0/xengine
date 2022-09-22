#include <gtest/gtest.h>

#include <entitysubsystem.h>

TEST(placeholdersuite, placeholdertest) {
  auto entity = EntitySubsystem::instance().create_entity();
  EXPECT_EQ(entity, 0);
  ASSERT_EQ(7 * 6, 42);
}
