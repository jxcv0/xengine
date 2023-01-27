#include <cstdint>
#include <cstdio>
#include <gtest/gtest.h>
#include <stdint.h>

#include "id_list.h"

TEST(entitiestests, create_id) {
  uint32_t id_list[128];
  uint32_t ctr = 0;
  init_id_list(id_list, 128);
  uint32_t id = pop_id(id_list, &ctr, 128);
  ASSERT_NE(id, 0);
  ASSERT_EQ(id, 1);
}

TEST(entitiestests, max_size) {
  uint32_t id_list[5];
  uint32_t ctr = 0;
  init_id_list(id_list, 5);
  uint32_t id;
  id = pop_id(id_list, &ctr, 5);
  ASSERT_EQ(id, 1);
  id = pop_id(id_list, &ctr, 5);
  ASSERT_EQ(id, 2);
  id = pop_id(id_list, &ctr, 5);
  ASSERT_EQ(id, 3);
  id = pop_id(id_list, &ctr, 5);
  ASSERT_EQ(id, 4);
  id = pop_id(id_list, &ctr, 5);
  ASSERT_EQ(id, 5);
  id = pop_id(id_list, &ctr, 5);
  ASSERT_EQ(id, 0);
}

TEST(entitiestests, delete_id) {
  uint32_t id_list[128];
  uint32_t ctr = 0;
  init_id_list(id_list, 128);
  uint32_t id1 = pop_id(id_list, &ctr, 128);
  ASSERT_EQ(id1, 1);
  uint32_t id2 = pop_id(id_list, &ctr, 128);
  ASSERT_EQ(id2, 2);
  uint32_t id3 = pop_id(id_list, &ctr, 128);
  ASSERT_EQ(id3, 3);
  uint32_t id4 = pop_id(id_list, &ctr, 128);
  ASSERT_EQ(id4, 4);
  push_id(id_list, id2, &ctr, 128); 
  push_id(id_list, id1, &ctr, 128); 
  push_id(id_list, id4, &ctr, 128); 
  uint32_t id5 = pop_id(id_list, &ctr, 128);
  ASSERT_EQ(id5, 4);
  uint32_t id6 = pop_id(id_list, &ctr, 128);
  ASSERT_EQ(id6, 1);
  uint32_t id7 = pop_id(id_list, &ctr, 128);
  ASSERT_EQ(id7, 2);
}
