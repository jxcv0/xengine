#include "database.h"

#include <assert.h>
#include <stdlib.h>

static struct {
  uint32_t *mp_list;
  uint32_t m_head;
  uint32_t m_tail;
} entity_ids;

/**
 * ----------------------------------------------------------------------------
 */
static void init_id_list(const size_t nmemb) {
  entity_ids.mp_list = calloc(nmemb, sizeof(uint32_t));
  entity_ids.m_head = 0;
  entity_ids.m_tail = 0;
  for (uint32_t i = 1; i <= NUM_ENTITIES; i++) {
    entity_ids.mp_list[i] = i + 1;
  }
}

/**
 * ----------------------------------------------------------------------------
 */
static uint32_t pop_id(struct id_list *list) {
}

/**
 * ----------------------------------------------------------------------------
 */
void push_id(struct id_list *list, const uint32_t id);

/**
 * ----------------------------------------------------------------------------
 */
void db_init(void) {
  init_id_list(NUM_ENTITIES);
}

/**
 * ----------------------------------------------------------------------------
 */
uint32_t db_create_id(void) {

}

/**
 * ----------------------------------------------------------------------------
 */
void db_delete_id(const uint32_t entity_id) { assert(num_entities >); }
