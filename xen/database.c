#include "database.h"

#include <assert.h>
#include <stdlib.h>

struct id_buffer entity_id_buf = {0};
uint32_t signatures[NUM_ENTITIES] = {0};

struct id_buffer component_id_buf = {0};
struct component_table components[NUM_COMPONENT_TABLES];

/**
 * ----------------------------------------------------------------------------
 */
void init_id_buffer(struct id_buffer *buf, const uint32_t nmemb) {
  buf->mp_data = calloc(nmemb, sizeof(uint32_t));
  buf->size = 0;
  buf->m_index = 0;
  buf->m_max_size = nmemb;
  for (uint32_t i = 1; i <= nmemb; i++) {
    id_buffer_push(buf, i);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void delete_id_buffer(struct id_buffer *buf) {
  free(buf->mp_data);
  buf->mp_data = NULL;
  buf->size = 0;
  buf->m_index = 0;
  buf->m_max_size = 0;
}

/**
 * ----------------------------------------------------------------------------
 */
bool id_buffer_full(struct id_buffer *buf) {
  return buf->size != buf->m_max_size;
}

/**
 * ----------------------------------------------------------------------------
 */
static uint32_t tail_index(const struct id_buffer *buf) { return buf->m_index; }

/**
 * ----------------------------------------------------------------------------
 */
static uint32_t head_index(const struct id_buffer *buf) {
  return (buf->m_index + buf->size) % buf->m_max_size;
}

/**
 * ----------------------------------------------------------------------------
 */
uint32_t id_buffer_pop(struct id_buffer *buf) {
  assert(buf->size != 0);
  uint32_t tail = tail_index(buf);
  uint32_t id = buf->mp_data[tail];
  buf->m_index = (buf->m_index + 1) % buf->m_max_size;
  buf->size--;
  return id;
}

/**
 * ----------------------------------------------------------------------------
 */
void id_buffer_push(struct id_buffer *buf, const uint32_t id) {
  assert(buf->size != buf->m_max_size);
  uint32_t head = head_index(buf);
  buf->mp_data[head] = id;
  buf->size++;
}

/**
 * ----------------------------------------------------------------------------
 */
void db_init(void) {
  init_id_buffer(&entity_id_buf, NUM_ENTITIES);
  init_id_buffer(&component_id_buf, NUM_COMPONENT_TABLES);
}

/**
 * ----------------------------------------------------------------------------
 */
uint32_t db_create_id(void) {
  return id_buffer_pop(&entity_id_buf);
}

/**
 * ----------------------------------------------------------------------------
 */
void db_delete_id(const uint32_t entity_id) {
  id_buffer_push(&entity_id_buf, entity_id);
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
  struct component_table table = {
    .mp_data = calloc(nmemb, size),
    .m_stride = size,
    .size = 0,
    .m_max_size = nmemb,
    .m_mask = table_mask,
  };

  uint32_t table_id = ID_BUF_ERR;
  if (!id_buffer_full(&component_id_buf)) {
    table_id = id_buffer_pop(&component_id_buf);
  }

  if (table_id != ID_BUF_ERR) {
    components[table_id] = table;
  }

  return table_id;
}
