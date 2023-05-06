#include <assert.h>
#include <stdio.h>

#include "ecs.h"
#include <math.h>
#include <float.h>

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

  assert(ecs_identity(e1) == GEOMETRY_BIT);
  assert(ecs_identity(e2) == MATERIAL_BIT);
  assert(ecs_identity(e3) == (GEOMETRY_BIT | MATERIAL_BIT));
  assert(ecs_identity(e4) == (GEOMETRY_BIT | MATERIAL_BIT | POSITION_BIT));
  assert(ecs_component_count(GEOMETRY) == 3);
  assert(ecs_component_count(MATERIAL) == 3);
  assert(ecs_component_count(POSITION) == 1);

  ecs_remove_component(e2, GEOMETRY); // should do nothing

  assert(ecs_identity(e2) == MATERIAL_BIT);
  ecs_remove_component(e4, MATERIAL);
  assert(ecs_identity(e4) == (GEOMETRY_BIT | POSITION_BIT));
  ecs_add_component(e2, POSITION);
  ecs_add_component(e1, POSITION);
  assert(ecs_identity(e1) == (GEOMETRY_BIT | POSITION_BIT));
  assert(ecs_identity(e2) == (MATERIAL_BIT | POSITION_BIT));
  assert(ecs_component_count(GEOMETRY) == 3);
  assert(ecs_component_count(MATERIAL) == 2);
  assert(ecs_component_count(POSITION) == 3);

  struct position pos1 = {{{1, 1, 1}}}; // this is wild
  struct position pos2 = {{{2, 2, 2}}};
  struct position pos3 = {{{3, 3, 3}}};
  struct position pos4 = {{{4, 4, 4}}};
  ecs_set_component(e1, POSITION, &pos1);
  ecs_set_component(e2, POSITION, &pos2);
  ecs_set_component(e3, POSITION, &pos3);
  ecs_set_component(e4, POSITION, &pos4);

  struct position val;
  assert(ecs_component(e1, POSITION, &val) != -1);

  for (int i = 0; i < 3; i++) {
    printf("%f, %f\n", val.vec[i], pos1.vec[i]);
    assert(fabs(val.vec[i] - pos1.vec[i]) < FLT_EPSILON);
  };
}
