#ifndef ECS_H_
#define ECS_H_

#include <stddef.h>

#include "types.h"

#define MAX_COMPONENT_TYPES 31

#define MAX_NUM_GEOMETRIES 32
#define MAX_NUM_MATERIALS 32
#define MAX_NUM_POSITIONS 32

#define GEOMETRY (1 << 0)
#define MATERIAL (1 << 1)
#define POSITION (1 << 2)

int ecs_init(size_t nent);

void ecs_shutdown(void);

int ecs_create_entity(uint32_t *e);

void ecs_delete_entity(uint32_t e);

uint32_t ecs_archetype(uint32_t e);

int ecs_add_component(uint32_t e, uint32_t type);

void *ecs_component(uint32_t e, uint32_t type);

int ecs_set_component(uint32_t e, uint32_t type, const void *val);

// TODO systems?

handle_t new_handle(void);

void free_handle(handle_t handle);

#endif  // ECS_H_
