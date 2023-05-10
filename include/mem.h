#ifndef MEM_H_
#define MEM_H_

#include <stddef.h>

#include "types.h"
// change to wider ints here if more required
typedef uint32_t cmpntmask_t;
typedef uint16_t handle_t;
#define MAX_NUM_ENTITIES UINT16_MAX

#define NUM_COMPONENT_TYPES ((sizeof(cmpntmask_t) * 8) - 1)
#define MASK_UNUSED (1 << NUM_COMPONENT_TYPES)

#define MAX_NUM_GEOMETRIES 32
#define MAX_NUM_MATERIALS 32
#define MAX_NUM_POSITIONS 32
#define MAX_NUM_ASSETS 32

#define GEOMETRY 0
#define MATERIAL 1
#define POSITION 2
#define QUEUED_ASSET 3

#define GEOMETRY_BIT (1 << GEOMETRY)
#define MATERIAL_BIT (1 << MATERIAL)
#define POSITION_BIT (1 << POSITION)
#define QUEUED_ASSET_BIT (1 << QUEUED_ASSET)

union component {
  struct geometry geometry;
  struct pbr_material material;
  struct position position;
  struct asset asset;
  // ...
};

void mem_init(void);

int mem_create_entity(uint32_t *e);

void mem_delete_entity(uint32_t e);

cmpntmask_t mem_mask(uint32_t e);

int mem_add_component(uint32_t e, uint32_t type);

void mem_remove_component(uint32_t e, uint32_t type);

// TODO Remove this function. Use _array and _write
union component *mem_component(uint32_t e, uint32_t type);

size_t mem_component_count(uint32_t type);

size_t mem_count(cmpntmask_t mask);

void mem_entities(cmpntmask_t mask, uint32_t *arr);

void mem_array(size_t nent, uint32_t *entities, uint32_t type,
               union component *array);

void mem_write(size_t nent, uint32_t *entities, uint32_t type,
               union component *array);

#endif  // MEM_H_
