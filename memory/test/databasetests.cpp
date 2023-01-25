#include <cstdint>
#include <cstdio>
#include <gtest/gtest.h>

#include "database.h"

int random_index() {
  return rand() % (MAX_IDS) + 1;
}

TEST(entitiestests, create_id) {
  db_init();
  uint32_t id = create_id();
  ASSERT_NE(id, DB_ERROR);
  ASSERT_EQ(id, 1);
  delete_id(id);
}

TEST(entitiestests, delete_id) {
  db_init();
  uint32_t id1 = create_id();
  ASSERT_EQ(id1, 1);
  uint32_t id2 = create_id();
  ASSERT_EQ(id2, 2);
  uint32_t id3 = create_id();
  ASSERT_EQ(id3, 3);
  uint32_t id4 = create_id();
  ASSERT_EQ(id4, 4);
  delete_id(id2); 
  delete_id(id1); 
  delete_id(id4); 
  uint32_t id5 = create_id();
  ASSERT_EQ(id5, 4);
  uint32_t id6 = create_id();
  ASSERT_EQ(id6, 1);
  uint32_t id7 = create_id();
  ASSERT_EQ(id7, 2);

  delete_id(id3);
  delete_id(id5);
  delete_id(id6);
  delete_id(id7);
}
