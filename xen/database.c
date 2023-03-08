#include "database.h"

#include <assert.h>
#include <stdlib.h>

/**
 * ----------------------------------------------------------------------------
 */
eid_t create_entity(void) {
  static eid_t id_counter = 0;
  assert(id_counter != MAX_NUM_ENTITIES);
  return id_counter++;
}
