#ifndef ID_LIST_H_
#define ID_LIST_H_

#include <stdint.h>

#define MAX_IDS 1024

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Set up an id list.
 *
 * @param id_list An array of ID's to initialize.
 * @param n The maximum number of elements in id_list.
 */
void init_id_list(uint32_t *id_list, const uint32_t n);

/**
 * @brief Get the next available ID from the list
 *
 * @param id_list The list to get the next id from.
 * @param c The number of ID's in the list.
 * @param n The maximum number of elements in id_list.
 * @return The next id or 0 if none is available.
 */
uint32_t pop_id(uint32_t *id_list, uint32_t *c, const uint32_t n);

/**
 * @brief Return an id to the free list.
 *
 * @param id The id to return.
 * @param c The number of ID's in the list.
 * @param n The maximum number of elements in id_list.
 */
void push_id(uint32_t *id_list, const uint32_t id, uint32_t *c,
             const uint32_t n);

#ifdef __cplusplus
}
#endif

#endif  // ID_LIST_H_
