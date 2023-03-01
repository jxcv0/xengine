#ifndef DATABASE_H_
#define DATABASE_H_

#include "assets.h"

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

#endif  // DATABASE_H_
