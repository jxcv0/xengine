#include "database.h"

#include <assert.h>
#include <stdio.h>

static uint32_t id_list[MAX_IDS];
static uint32_t current_id_index = 0;

/**
 * ----------------------------------------------------------------------------
 */
void db_init(void) {
  for (int i = MAX_IDS; i > 0; i--) {
    id_list[i - 1] = i;
  }
}

/**
 * ----------------------------------------------------------------------------
 */
uint32_t create_id(void) {
  if (current_id_index != MAX_IDS) {
    return id_list[current_id_index++];
  }
  return DB_ERROR;
}

/**
 * ----------------------------------------------------------------------------
 */
void delete_id(const uint32_t id) {
  assert(id > 0 && id < MAX_IDS + 1);
  assert(current_id_index > 0 && current_id_index < MAX_IDS);
  id_list[--current_id_index] = id;
}
