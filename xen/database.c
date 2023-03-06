#include "database.h"

#include <assert.h>
#include <stdlib.h>

static struct id_buffer entity_id_buf = {0};
static uint32_t signatures[NUM_ENTITIES] = {0};
static struct id_buffer table_ids = {0};

/**
 * ----------------------------------------------------------------------------
 */
void init_id_buffer(struct id_buffer *buf, const uint32_t nmemb) {
  buf->mp_data = calloc(nmemb, sizeof(uint32_t));
  buf->m_size = 0;
  buf->m_index = 0;
  buf->m_max_size = nmemb;
  for (uint32_t i = 1; i <= nmemb; i++) {
    push_id(buf, i);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void delete_id_buffer(struct id_buffer *buf) {
  free(buf->mp_data);
  buf->mp_data = NULL;
  buf->m_size = 0;
  buf->m_index = 0;
  buf->m_max_size = 0;
}

/**
 * ----------------------------------------------------------------------------
 */
static uint32_t tail_index(const struct id_buffer *buf) { return buf->m_index; }

/**
 * ----------------------------------------------------------------------------
 */
static uint32_t head_index(const struct id_buffer *buf) {
  return (buf->m_index + buf->m_size) % buf->m_max_size;
}

/**
 * ----------------------------------------------------------------------------
 */
uint32_t pop_id(struct id_buffer *buf) {
  assert(buf->m_size != 0);
  uint32_t tail = tail_index(buf);
  uint32_t id = buf->mp_data[tail];
  buf->m_index = (buf->m_index + 1) % buf->m_max_size;
  buf->m_size--;
  return id;
}

/**
 * ----------------------------------------------------------------------------
 */
void push_id(struct id_buffer *buf, const uint32_t id) {
  assert(buf->m_size != buf->m_max_size);
  uint32_t head = head_index(buf);
  buf->mp_data[head] = id;
  buf->m_size++;
}

/**
 * ----------------------------------------------------------------------------
 */
void db_init(void) { init_id_buffer(&entity_id_buf, NUM_ENTITIES); }

/**
 * ----------------------------------------------------------------------------
 */
uint32_t db_create_id(void) { return pop_id(&entity_id_buf); }

/**
 * ----------------------------------------------------------------------------
 */
void db_delete_id(const uint32_t entity_id) {
  push_id(&entity_id_buf, entity_id);
  // TODO free entries in component arrays
}

/**
 * ----------------------------------------------------------------------------
 */
uint32_t db_get_signature(const uint32_t entity_id) {
  return signatures[entity_id];
}

/**
 * ----------------------------------------------------------------------------
 */
uint32_t db_create_table(const size_t nmemb, const size_t size,
                         uint32_t table_mask) {
  return 0;
}
