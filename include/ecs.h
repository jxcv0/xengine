#ifndef ECS_H_
#define ECS_H_

// TODO rename this. Not really an ecs. just cache friendly memory managment and
// access.

#include <stddef.h>

#include "types.h"

#define MAX_NUM_ENTITIES 128

// Can be changed to 63 if uint64_t is used
#define NUM_COMPONENT_TYPES 31

#define MAX_NUM_GEOMETRIES 32
#define MAX_NUM_MATERIALS 32
#define MAX_NUM_POSITIONS 32
#define MAX_NUM_ASSETS 32

// lookup index
#define GEOMETRY 0
#define MATERIAL 1
#define POSITION 2
#define QUEUED_ASSET 3

// bitmasks
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

void ecs_init(void);

int ecs_create_entity(uint32_t *e);

void ecs_delete_entity(uint32_t e);

uint32_t ecs_identity(uint32_t e);

int ecs_add_component(uint32_t e, uint32_t type);

void ecs_remove_component(uint32_t e, uint32_t type);

// TODO Remove this function. Use _array and _write
union component *ecs_component(uint32_t e, uint32_t type);

size_t ecs_component_count(uint32_t type);

size_t ecs_count(uint32_t mask);

void ecs_entities(uint32_t mask, uint32_t *arr);

void ecs_array(size_t nent, uint32_t *entities, uint32_t type,
               union component *array);

void ecs_write(size_t nent, uint32_t *entities, uint32_t type,
               union component *array);

#endif  // ECS_H_
