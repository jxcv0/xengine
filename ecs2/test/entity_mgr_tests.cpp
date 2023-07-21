#include <gtest/gtest.h>

#include "entity_mgr.hpp"

TEST(entity_mgr_tests, create_and_delete_entities) {
  xen::entity_mgr mgr;
  xen::eid_t e1 = mgr.create_entity();
  ASSERT_EQ(e1, 0);
  ASSERT_EQ(mgr.num_entities(), 1);

  mgr.delete_entity(e1);
  ASSERT_EQ(mgr.num_entities(), 0);
  xen::eid_t e2 = mgr.create_entity();
  ASSERT_EQ(mgr.num_entities(), 1);
  ASSERT_EQ(e1, e2); /* ID should be reused */
}

TEST(entity_mgr_tests, is_valid_id) {
  xen::entity_mgr mgr;
  xen::eid_t e1 = mgr.create_entity();
  ASSERT_TRUE(mgr.is_valid_id(e1));
  ASSERT_FALSE(mgr.is_valid_id(-1));
  mgr.delete_entity(e1);
  ASSERT_FALSE(mgr.is_valid_id(e1));
}