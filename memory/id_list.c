#include "id_list.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

/**
 * ----------------------------------------------------------------------------
 */
void init_id_list(uint32_t *list, const uint32_t n) {
  for (uint32_t i = 0; i < n; i++) {
    list[i] = i + 1;
  }
}

/**
 * ----------------------------------------------------------------------------
 */
uint32_t pop_id(uint32_t *id_list, uint32_t *c, const uint32_t n) {
  if (*c < n) {
    return id_list[(*c)++];
  }
  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
void push_id(uint32_t *id_list, const uint32_t id, uint32_t *c,
             const uint32_t n) {
  assert(id > 0 && id < n + 1);
  assert(*c > 0 && *c < n);
  id_list[--(*c)] = id;
}
