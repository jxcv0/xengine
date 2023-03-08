#include "database.h"

#include <assert.h>
#include <stdlib.h>

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
  return buf->size == buf->m_max_size;
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
eid_t create_entity(void) {
  static eid_t id_counter = 0;
  assert(id_counter != MAX_NUM_ENTITIES);
  return id_counter++;
}
