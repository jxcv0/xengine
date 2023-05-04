#ifndef ECS_H_
#define ECS_H_

#include <stddef.h>

#include "types.h"

// First bit is used to specify if id is unused
#define GEOMETRY (1 << 0)
#define MATERIAL (1 << 1)
#define POSITION (1 << 2)

#define ECSERR_SYS 1

int ecs_init(size_t nent);

void ecs_shutdown(void);

int ecs_create_entity(uint32_t *e);

void ecs_delete_entity(uint32_t e);

int ecs_add_component(uint32_t e, uint32_t type);

void *ecs_component_value(uint32_t e, uint32_t type);

int ecs_set_component(uint32_t e, uint32_t type, const void *val, size_t size);

// TODO systems?

handle_t new_handle(void);

void free_handle(handle_t handle);

#endif  // ECS_H_
