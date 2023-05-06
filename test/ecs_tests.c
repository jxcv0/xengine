#include <assert.h>
#include <stdio.h>

#include "ecs.h"

int main() {
  ecs_init();
  uint32_t e1;
  assert(ecs_identity(0) == 0x80000000);
  assert(ecs_create_entity(&e1) != -1);
  assert(ecs_identity(e1) == 0);
  ecs_delete_entity(e1);
  assert(ecs_identity(e1) == 0x80000000);

  ecs_create_entity(&e1);
  assert(ecs_add_component(e1, GEOMETRY) != -1);
  assert(ecs_identity(e1) == 0x1);

  uint32_t e2, e3, e4;
  ecs_create_entity(&e2);
  ecs_create_entity(&e3);
  ecs_create_entity(&e4);

  ecs_add_component(e2, MATERIAL);

  ecs_add_component(e3, GEOMETRY);
  ecs_add_component(e3, MATERIAL);

  ecs_add_component(e4, GEOMETRY);
  ecs_add_component(e4, MATERIAL);
  ecs_add_component(e4, POSITION);

  /*
  assert(ecs_identity(e1) == GEOMETRY);
  assert(ecs_identity(e2) == MATERIAL);
  assert(ecs_identity(e3) == (GEOMETRY | MATERIAL));
  assert(ecs_identity(e4) == (GEOMETRY | MATERIAL | POSITION));
  */
}
