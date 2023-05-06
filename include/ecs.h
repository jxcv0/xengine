#ifndef ECS_H_
#define ECS_H_

#include <stddef.h>

#include "types.h"

#define MAX_NUM_ENTITIES 128

// Can be changed to 63 if uint64_t is used
#define NUM_COMPONENT_TYPES 31

#define MAX_NUM_GEOMETRIES 32
#define MAX_NUM_MATERIALS 32
#define MAX_NUM_POSITIONS 32

#define GEOMETRY 0
#define MATERIAL 1
#define POSITION 2

#define GEOMETRY_BIT (1 << 0)
#define MATERIAL_BIT (1 << 1)
#define POSITION_BIT (1 << 2)

void ecs_init(void);

void ecs_shutdown(void);

int ecs_create_entity(uint32_t *e);

void ecs_delete_entity(uint32_t e);

uint32_t ecs_identity(uint32_t e);

int ecs_add_component(uint32_t e, uint32_t type);

void ecs_remove_component(uint32_t e, uint32_t type);

int ecs_component(uint32_t e, uint32_t type, void *val);

int ecs_set_component(uint32_t e, uint32_t type, const void *val);

size_t ecs_component_count(uint32_t type);

// TODO systems?

handle_t new_handle(void);

void free_handle(handle_t handle);

#endif  // ECS_H_
