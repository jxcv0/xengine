#ifndef DATABASE_H_
#define DATABASE_H_

#include "assets.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the database.
 * @param num_entities The maximum number of entities.
 */
void db_init(const uint32_t num_entities);

/**
 * @brief Release aquired resources.
 */
void db_cleanup(void);

/**
 * @brief Create an entity.
 *
 * @return The new entity id.
 */
uint32_t create_entity();

const struct geometry *geometries_begin();
const struct geometry *geometries_end();

struct geometry *get_geometry(uint32_t entity_id);
struct pbr_material *get_material(uint32_t entity_id);

#ifdef __cplusplus
}
#endif

#endif  // DATABASE_H_
