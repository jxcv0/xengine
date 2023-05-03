#ifndef ECS_H_
#define ECS_H_

#include <stddef.h>

#include "types.h"

#define MAX_NUM_ENTITIES 128

#define GEOMETRY (1 << 0)
#define MATERIAL (1 << 1)
#define POSITION (1 << 2)

/**
 * @brief Create a new handle.
 *
 * @return A new handle.
 */
handle_t new_handle(void);

void free_handle(handle_t handle);

#endif  // ECS_H_
