#ifndef DATABASE_H_
#define DATABASE_H_

#include <stdbool.h>

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Circular buffer for id's.
 */
struct id_buffer {
  uint32_t *mp_data;
  uint32_t size;
  uint32_t m_index;
  uint32_t m_max_size;
};

struct database {
  struct id_buffer m_free_entities;
};

/**
 * @brief Initialize an id_buffer with memory for nmemb members.
 *
 * @param buf A pointer to the buffer to initialize.
 * @param nmemb The the number of members in the id buffer.
 */
void init_id_buffer(struct id_buffer *buf, const uint32_t nmemb);

/**
 * @breif Release resources in use by an id buffer.
 *
 * @param buf A pointer to the buffer to delete.
 */
void delete_id_buffer(struct id_buffer *buf);

/**
 * @brief Get the next id from the buffer.
 *
 * @param buf A pointer to the buffer to fetch the id from.
 * @return The id.
 */
uint32_t id_buffer_pop(struct id_buffer *buf);

/**
 * @brief Push an id onto a buffer.
 *
 * @param buf A pointer to the buffer.
 * @param id The id to store.
 */
void id_buffer_push(struct id_buffer *buf, const uint32_t id);

/**
 * @brief Check if a buffer is full.
 *
 * @param buf A pointer to the buffer to check.
 */
bool id_buffer_full(struct id_buffer *buf);

/**
 * @brief Create an entity id.
 */
eid_t create_entity(void);

/**
 * @brief Delete all reference to an entity id
 * @param entity The id of the entity to delete.
 */
void delete_entity(const eid_t entity);

#ifdef __cplusplus
}
#endif

#endif  // DATABASE_H_
