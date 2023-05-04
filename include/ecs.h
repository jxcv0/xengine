#ifndef ECS_H_
#define ECS_H_

#include <stddef.h>

#include "types.h"

#define MAX_NUM_ENTITIES 128

#define GEOMETRY (1 << 0)
#define MATERIAL (1 << 1)
#define POSITION (1 << 2)

int ecs_init();

void ecs_shutdown();

int ecs_err();

int ecs_create_handle(handle_t *h);

int ecs_add_component(handle_t h, uint32_t type);

void *ecs_component_value(handle_t h, uint32_t type);

int ecs_set_component(handle_t h, uint32_t type, const void *val, size_t size);

handle_t new_handle(void);

void free_handle(handle_t handle);

#endif  // ECS_H_
