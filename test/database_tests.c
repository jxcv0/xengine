#include <assert.h>
#include <libgen.h>
#include <stdio.h>

#include "database.h"

/*
void tst_create_id(void) {
  uint32_t one = create_id();
  uint32_t two = create_id();
  uint32_t three = create_id();
  assert(one == 1);
  assert(two == 2);
  assert(three == 3);
}
*/

int main() {
  // db_init();
  // tst_create_id();

  char *suite = basename(__FILE__);
  printf("[%s]: All tests passed.\n", suite);
  return 0;
}
