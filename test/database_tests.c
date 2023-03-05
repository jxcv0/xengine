#include <assert.h>
#include <libgen.h>
#include <stdio.h>

#include "database.h"

void tst_init_id_buffer(void) {
  struct id_buffer buf = {0};
  init_id_buffer(&buf, 64);
  assert(buf.mp_data != NULL);
  assert(buf.m_size == 64);
  assert(buf.m_index == 0);
  assert(buf.m_max_size == 64);
}

void tst_pop_push_id(void) {
  struct id_buffer buf = {0};
  init_id_buffer(&buf, 3);
  uint32_t id1 = pop_id(&buf);
  uint32_t id2 = pop_id(&buf);
  uint32_t id3 = pop_id(&buf);
  assert(id1 == 1);
  assert(id2 == 2);
  assert(id3 == 3);
  push_id(&buf, id3);
  push_id(&buf, id2);
  push_id(&buf, id1);
  id1 = pop_id(&buf);
  id2 = pop_id(&buf);
  id3 = pop_id(&buf);
  assert(id1 == 3);
  assert(id2 == 2);
  assert(id3 == 1);
}

int main() {
  tst_init_id_buffer();
  tst_pop_push_id();

  char *suite = basename(__FILE__);
  printf("[%s]: All tests passed.\n", suite);
  return 0;
}
