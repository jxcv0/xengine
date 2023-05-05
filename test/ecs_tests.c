#include <assert.h>
#include <stdio.h>

#include "ecs.h"

int main() {
  assert(ecs_init(16) != -1);

  uint32_t e1;
  assert(ecs_archetype(0) == 0x80000000);
  assert(ecs_create_entity(&e1) != -1);
  assert(ecs_archetype(e1) == 0);
  ecs_delete_entity(e1);
  assert(ecs_archetype(e1) == 0x80000000);
}
