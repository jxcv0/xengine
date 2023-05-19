#ifndef MEM_H_
#define MEM_H_

#include <stddef.h>

#include "types.h"

// change to wider int here if more than 31 components required
typedef uint32_t cmpnt_t;

#define NUM_COMPONENT_TYPES ((sizeof(cmpnt_t) * 8) - 1)
#define MAX_NUM_ENTITIES 2048
#define ENTITY_UNUSED (1 << NUM_COMPONENT_TYPES)

#define MAX_NUM_GEOMETRIES 32
#define MAX_NUM_MATERIALS 32
#define MAX_NUM_POSITIONS 32
#define MAX_NUM_MODEL_MATRICES 32
#define MAX_NUM_LOAD_REQUESTS 32

// lookup index
#define GEOMETRY 0
#define MATERIAL 1
#define MODEL_MATRIX 2
#define POSITION 3
#define LOAD_REQUEST 4

// bitmasks
#define GEOMETRY_BIT (1 << GEOMETRY)
#define MATERIAL_BIT (1 << MATERIAL)
#define MODEL_MATRIX_BIT (1 << MODEL_MATRIX)
#define POSITION_BIT (1 << POSITION)
#define LOAD_REQUEST_BIT (1 << LOAD_REQUEST)

union component {
  struct geometry geometry;
  struct pbr_material material;
  struct position position;
  struct model_matrix model_matrix;
  struct loadreq load;
  // ...
};

/**
 * @brief Initialize the memory subsystem.
 */
void mem_init(void);

/**
 * @brief Create an entity.
 * @param e A pointer to the entity handle to initialize.
 * @return 0 on success, -1 on failure.
 */
int mem_create_entity(uint32_t *e);

/**
 * @brief Delete an entity.
 * @details If the entity does not exist then this function does nothing.
 */
void mem_delete_entity(uint32_t e);

/**
 * @brief Get the identity (component mask) of an entity.
 * @param e The entity.
 * @return A mask of all the components this entity has an instance of.
 */
cmpnt_t mem_identity(uint32_t e);

int mem_add_component(uint32_t e, cmpnt_t type);

void mem_remove_component(uint32_t e, cmpnt_t type);

// TODO Remove this function. Use _array and _write
union component *mem_component(uint32_t e, cmpnt_t type);

size_t mem_component_count(uint32_t cmpnt_t);

/**
 * @brief Count the number of entities who's identity matches mask.
 * @param Mask The component bitmask.
 * @return The number of entities.
 */
size_t mem_count(cmpnt_t mask);

void mem_entities(cmpnt_t mask, uint32_t *arr);

void mem_array(size_t nent, uint32_t *entities, cmpnt_t type,
               union component *array);

void mem_write(size_t nent, uint32_t *entities, cmpnt_t type,
               union component *array);

#endif  // MEM_H_
