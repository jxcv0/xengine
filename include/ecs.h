#ifndef ECS_H_
#define ECS_H_

#include <stddef.h>

#include "types.h"

#define MAX_NUM_ENTITIES 128

// Can be changed to 63 if uint64_t is used
#define MAX_COMPONENT_TYPES 31

#define MAX_NUM_GEOMETRIES 32
#define MAX_NUM_MATERIALS 32
#define MAX_NUM_POSITIONS 32

enum component_type { GEOMETRY = 0, MATERIAL = 1, POSITION = 2 };

void ecs_init(void);

void ecs_shutdown(void);

int ecs_create_entity(uint32_t *e);

void ecs_delete_entity(uint32_t e);

uint32_t ecs_archetype(uint32_t e);

int ecs_add_component(uint32_t e, enum component_type type);

void ecs_remove_component(uint32_t e, enum component_type type);

void *ecs_component(uint32_t e, enum component_type type);

int ecs_set_component(uint32_t e, enum component_type type, const void *val);

// TODO systems?

handle_t new_handle(void);

void free_handle(handle_t handle);

#endif  // ECS_H_
