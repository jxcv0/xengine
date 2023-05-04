#include "ecs.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

static int ecs_errno;
static handle_t counter = 1;

static struct {
  uint32_t *data;
  size_t count;
  size_t max;
} entity_buf;

/**
 * ----------------------------------------------------------------------------
 */
int ecs_init(size_t nent) {
  entity_buf.data = calloc(nent, sizeof(uint32_t));
  if (entity_buf.data == NULL) {
    ecs_errno = ECSERR_SYS;
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
  if (entity_buf.data == NULL || e == NULL ||
      entity_buf.count == entity_buf.max) {
    return -1;
  }

  for (size_t i = 0; i < entity_buf.max; i++) {
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
handle_t new_handle(void) {
  assert(counter != UINT64_MAX);
  return counter++;
}
