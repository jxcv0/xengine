#include "ecs.h"

#include <assert.h>

/**
 * ----------------------------------------------------------------------------
 */
handle_t new_handle(void) {
  static handle_t counter = 1;
  assert(counter != UINT64_MAX);
  return counter++;
}

