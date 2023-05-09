#include <assert.h>
#include <float.h>
#include <math.h>
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

  assert(ecs_add_component(e2, MATERIAL) != -1);

  assert(ecs_add_component(e3, GEOMETRY) != -1);
  assert(ecs_add_component(e3, MATERIAL) != -1);

  assert(ecs_add_component(e4, GEOMETRY) != -1);
  assert(ecs_add_component(e4, MATERIAL) != -1);
  assert(ecs_add_component(e4, POSITION) != -1);

  assert(ecs_identity(e1) == GEOMETRY_BIT);
  assert(ecs_identity(e2) == MATERIAL_BIT);
  assert(ecs_identity(e3) == (GEOMETRY_BIT | MATERIAL_BIT));
  assert(ecs_identity(e4) == (GEOMETRY_BIT | MATERIAL_BIT | POSITION_BIT));
  assert(ecs_component_count(GEOMETRY) == 3);
  assert(ecs_component_count(MATERIAL) == 3);
  assert(ecs_component_count(POSITION) == 1);

  assert(ecs_count(GEOMETRY_BIT) == 3);
  assert(ecs_count(GEOMETRY_BIT | MATERIAL_BIT) == 2);
  assert(ecs_count(GEOMETRY_BIT | MATERIAL_BIT | POSITION_BIT) == 1);

  ecs_remove_component(e2, GEOMETRY);  // should do nothing

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

  ecs_add_component(e3, POSITION);
  struct position pos1 = {{{1, 1, 1}}};  // this is wild
  struct position pos2 = {{{2, 2, 2}}};
  struct position pos3 = {{{3, 3, 3}}};
  struct position pos4 = {{{4, 4, 4}}};
  ecs_component(e1, POSITION)->position = pos1;
  ecs_component(e2, POSITION)->position = pos2;
  ecs_component(e3, POSITION)->position = pos3;
  ecs_component(e4, POSITION)->position = pos4;

  struct position p = ecs_component(e1, POSITION)->position;
  for (int i = 0; i < 3; i++) {
    assert(fabs(p.vec[i] - pos1.vec[i]) < 0.0001);
  };

  // removing item in middle of buffer
  ecs_remove_component(e2, POSITION);

  p = ecs_component(e1, POSITION)->position;
  for (int i = 0; i < 3; i++) {
    assert(fabs(p.vec[i] - pos1.vec[i]) < 0.0001);
  };

  p = ecs_component(e4, POSITION)->position;
  for (int i = 0; i < 3; i++) {
    assert(fabs(p.vec[i] - pos4.vec[i]) < 0.0001);
  };

  ecs_delete_entity(e1);
  ecs_delete_entity(e2);
  ecs_delete_entity(e3);
  ecs_delete_entity(e4);
  assert(ecs_count(GEOMETRY_BIT) == 0);
  assert(ecs_count(GEOMETRY_BIT | MATERIAL_BIT) == 0);
  assert(ecs_count(GEOMETRY_BIT | MATERIAL_BIT | POSITION_BIT) == 0);
  assert(ecs_component_count(GEOMETRY) == 0);
  assert(ecs_component_count(MATERIAL) == 0);
  assert(ecs_component_count(POSITION) == 0);

  ecs_create_entity(&e1);
  ecs_create_entity(&e2);
  ecs_create_entity(&e3);
  ecs_create_entity(&e4);

  ecs_add_component(e1, GEOMETRY);
  ecs_add_component(e2, GEOMETRY);
  ecs_add_component(e3, GEOMETRY);

  ecs_add_component(e1, MATERIAL);
  ecs_add_component(e2, MATERIAL);
  ecs_add_component(e3, MATERIAL);

  ecs_add_component(e1, POSITION);
  ecs_add_component(e2, POSITION);
  ecs_add_component(e3, POSITION);
  ecs_add_component(e4, POSITION);

  assert(ecs_component_count(GEOMETRY) == 3);
  assert(ecs_component_count(MATERIAL) == 3);
  assert(ecs_component_count(POSITION) == 4);

  assert(ecs_count(GEOMETRY_BIT) == 3);
  assert(ecs_count(GEOMETRY_BIT | POSITION_BIT) == 3);
  assert(ecs_count(GEOMETRY_BIT | MATERIAL_BIT) == 3);
  assert(ecs_count(GEOMETRY_BIT | MATERIAL_BIT | POSITION_BIT) == 3);
  assert(ecs_count(POSITION_BIT) == 4);

  size_t ne = ecs_count(GEOMETRY_BIT | POSITION_BIT) * 2;
  size_t arr_size = ne * 2;
  uint32_t e_arr[ne];
  ecs_entities(GEOMETRY_BIT | POSITION_BIT, e_arr);
  assert(e_arr[0] == e1);
  assert(e_arr[1] == e2);
  assert(e_arr[2] == e3);

  ecs_component(e1, GEOMETRY)->geometry.m_vbo = 1;
  ecs_component(e2, GEOMETRY)->geometry.m_vbo = 2;
  ecs_component(e3, GEOMETRY)->geometry.m_vbo = 3;
  ecs_component(e1, POSITION)->position.x = 1.0f;
  ecs_component(e2, POSITION)->position.x = 2.0f;
  ecs_component(e3, POSITION)->position.x = 3.0f;

  union component arr[arr_size];
}
