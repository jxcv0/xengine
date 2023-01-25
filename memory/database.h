#ifndef ENTITIES_H_
#define ENTITIES_H_

#include <stdint.h>

#define MAX_IDS 1024
#define DB_ERROR 0

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Set up db data.
 */
void db_init(void);

/**
 * @brief Create a key. 0 is not a valid id.
 *
 * @return The key.
 */
uint32_t create_id(void);

/**
 * @brief Delete an id and return it to the free list.
 *
 * @param entity_id The id to delete.
 */
void delete_id(const uint32_t id);

#ifdef __cplusplus
}
#endif

#endif  // ENTITIES_H_
