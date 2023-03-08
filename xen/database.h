#ifndef DATABASE_H_
#define DATABASE_H_

#include <stdbool.h>

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

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
