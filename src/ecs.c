#include "ecs.h"

#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

#define ID_UNUSED (1 << 31)
#define ID_MAX (UINT32_MAX - 1)

static struct {
  uint32_t *data;
  size_t count;
  size_t max;
} entity_buf;

static struct {
  struct geometry *data;
  size_t count;
  size_t max;
} geometry_buf;

/**
 * ----------------------------------------------------------------------------
 */
int ecs_init(size_t nent) {
  entity_buf.data = calloc(nent, sizeof(uint32_t));
  if (entity_buf.data == NULL) {
    return -1;
  }
  for (size_t i = 0; i < nent; i++) {
    entity_buf.data[i] = ID_UNUSED;
  }
  entity_buf.count = 0;
  entity_buf.max = nent;
  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
int ecs_create_entity(uint32_t *e) {
  if (entity_buf.data == NULL || e == NULL) {
      errno = EINVAL;
    return -1;
  }

  if (entity_buf.count == entity_buf.max) {
    // TODO errno?
    return -1;
  }

  for (size_t i = 0; i < entity_buf.count; i++) {
    if ((entity_buf.data[i] & ID_UNUSED) != 0) {
      *e = i;
      entity_buf.data[i] ^= ID_UNUSED;
      entity_buf.count++;
      return 0;
    }
  }
  return -1;
}

/**
 * ----------------------------------------------------------------------------
 */
void ecs_delete_entity(uint32_t e) {

    // TODO delete this entities entries in component buffers
    entity_buf.data[e] = ID_UNUSED;
}

/**
 * ----------------------------------------------------------------------------
 */
handle_t new_handle(void) {
  static handle_t counter = 1;
  return counter++;
}
