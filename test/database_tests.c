#include <libgen.h>
#include <stdio.h>
#include <assert.h>

#include "test.h"
#include "database.h"
#include "error.h"

int main() {
  eid_t e = create_entity();
  assert(e == 1);
  
  assign_component(e, GEOMETRY);
  assert(get_signature(e) & GEOMETRY);
  assert(!(get_signature(e) & MATERIAL));

  TEST_END();
  return 0;
}
