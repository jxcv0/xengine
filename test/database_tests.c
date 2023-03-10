#include <libgen.h>
#include <stdio.h>
#include <assert.h>

#include "database.h"

int main() {
  eid_t e = create_entity();
  assert(e == 1);
  
  assign_component(e, GEOMETRY);
  assert(get_signature(e) & GEOMETRY);
  return 0;
}
