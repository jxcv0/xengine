#ifndef DATABASE_H_
#define DATABASE_H_

#include <stddef.h>
#include <stdint.h>

#include "lin.h"
#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_ENTITY_ID 0xFFFF
#define MAX_NUM_ENTITIES 1024
#define MAX_COMPONENT_TYPES 32  // limited by with of uint32_t

#define MAX_NUM_GEOMETRIES 1028
#define MAX_NUM_MATERIALS 1028

typedef uint32_t eid_t;
typedef uint32_t sig_t;

struct component {
  eid_t m_entity;
  union {
    struct geometry m_geom;
    struct pbr_material m_mat;
    /* others */
  };
};

enum component_type { XEN_GEOMETRY = 0x0001, XEN_MATERIAL = 0x0002 };

/**
 * @brief Create an entity id.
 * @return The id of the entity.
 */
eid_t create_entity(void);

/**
 * @brief Delete all reference to an entity id.
 * @param entity The id of the entity to delete.
 */
void delete_entity(const eid_t id);

/**
 * @brief Get the signature of an entity
 * @param entity The id of the entity.
 * @return The signature of the entity.
 */
sig_t get_signature(const eid_t id);

/**
 * @brief Get the number of entities who have components sig.
 * @param sig A mask of the component signatures to search for.
 * @return The number of entities who have the components in sig.
 */
size_t count_entities(const sig_t sig);

/**
 * @brief Get the entity id of all entities with the components sig.
 * @param arr An array to store the results in. This array is assumed to be big
 * enough.
 * @param sig The mask of the components to include in the search.
 */
void get_entities(eid_t *arr, const sig_t sig);

/**
 * @brief Assign a component to an entity.
 * @param id The id of the entity.
 * @param sig A mask of the component signatures to add.
 */
void assign_components(const eid_t id, const sig_t sig);

/**
 * @brief Remove a components association with an entity.
 * @param id The id of the entity
 * @param sig A mask of the component signatures to remove.
 */
void remove_components(const eid_t id, const sig_t sig);

/**
 * @brief 
 * @param ids an array of ids to fetch the components of
 * @
 */
struct component *get_components(const eid_t *ids, const size_t n, const sig_t sig);

//
// get_components(ids, dest, num_entities, XEN_GEOMETRY)
//

#ifdef __cplusplus
}
#endif

#endif  // DATABASE_H_
