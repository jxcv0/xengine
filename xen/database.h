#ifndef DATABASE_H_
#define DATABASE_H_

#include "lin.h"
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct component {
  eid_t m_entity_id;
  union {
    struct pbr_material m_material;
    struct geometry m_geometry;
    vec3 m_position;
    vec3 m_velocity;
  };
};

uint32_t num_materials = 0;
struct pbr_material material_table[] = {0};

uint32_t num_geometries = 0;
struct geometry geometry_table[] = {0};

uint32_t num_positions = 0;
vec3 position_table[] = {0};

uint32_t num_velocities = 0;
vec3 velocity_table[] = {0};

uint32_t num_cameras = 0;
struct camera camera_table[] = {0};

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
