#include "ecs.h"

#include <assert.h>
#include <stdint.h>

static struct {
  handle_t handle;
  signature_t singature;
} signature_map[MAX_NUM_ENTITIES];

static size_t num_entities = 0;
static handle_t counter = 1;

/**
 * ----------------------------------------------------------------------------
 */
handle_t new_handle(void) {
  assert(counter != UINT64_MAX);
  handle_t h = counter++;
  signature_map[num_entities++].handle = h;
  return h;
}

/**
 * ----------------------------------------------------------------------------
 */
void free_handle(handle_t handle) {
  for (size_t i = 0; i < num_entities; i++) {
    if (signature_map[i].handle == handle) {
      signature_map[i] = signature_map[num_entities--];
    }
  }
}
