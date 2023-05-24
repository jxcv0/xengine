#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>

#include "mem.h"
#include "test.h"

int main() {
  TEST_BEGIN();
  TEST();
  mem_init();
  uint32_t e1;
  assert(mem_identity(0) == 0x80000000);
  assert(mem_create_entity(&e1) != -1);
  assert(mem_identity(e1) == 0);
  mem_delete_entity(e1);
  assert(mem_identity(e1) == 0x80000000);

  mem_create_entity(&e1);
  assert(mem_add_component(e1, GEOMETRY) != -1);
  assert(mem_identity(e1) == 0x1);

  uint32_t e2, e3, e4;
  mem_create_entity(&e2);
  mem_create_entity(&e3);
  mem_create_entity(&e4);

  assert(mem_add_component(e2, MATERIAL) != -1);

  assert(mem_add_component(e3, GEOMETRY) != -1);
  assert(mem_add_component(e3, MATERIAL) != -1);

  assert(mem_add_component(e4, GEOMETRY) != -1);
  assert(mem_add_component(e4, MATERIAL) != -1);
  assert(mem_add_component(e4, POSITION) != -1);

  assert(mem_identity(e1) == GEOMETRY_BIT);
  assert(mem_identity(e2) == MATERIAL_BIT);
  assert(mem_identity(e3) == (GEOMETRY_BIT | MATERIAL_BIT));
  assert(mem_identity(e4) == (GEOMETRY_BIT | MATERIAL_BIT | POSITION_BIT));
  assert(mem_component_count(GEOMETRY) == 3);
  assert(mem_component_count(MATERIAL) == 3);
  assert(mem_component_count(POSITION) == 1);

  assert(mem_count(GEOMETRY_BIT) == 3);
  assert(mem_count(GEOMETRY_BIT | MATERIAL_BIT) == 2);
  assert(mem_count(GEOMETRY_BIT | MATERIAL_BIT | POSITION_BIT) == 1);

  mem_remove_component(e2, GEOMETRY);  // should do nothing

  assert(mem_identity(e2) == MATERIAL_BIT);
  mem_remove_component(e4, MATERIAL);
  assert(mem_identity(e4) == (GEOMETRY_BIT | POSITION_BIT));
  mem_add_component(e2, POSITION);
  mem_add_component(e1, POSITION);
  assert(mem_identity(e1) == (GEOMETRY_BIT | POSITION_BIT));
  assert(mem_identity(e2) == (MATERIAL_BIT | POSITION_BIT));
  assert(mem_component_count(GEOMETRY) == 3);
  assert(mem_component_count(MATERIAL) == 2);
  assert(mem_component_count(POSITION) == 3);

  mem_add_component(e3, POSITION);
  union component pos1;
  pos1.position.x = 1.0f;
  pos1.position.y = 1.0f;
  pos1.position.z = 1.0f;

  union component pos2;
  pos2.position.x = 2.0f;
  pos2.position.y = 2.0f;
  pos2.position.z = 2.0f;

  union component pos3;
  pos3.position.x = 3.0f;
  pos3.position.y = 3.0f;
  pos3.position.z = 3.0f;

  union component pos4;
  pos4.position.x = 4.0f;
  pos4.position.y = 4.0f;
  pos4.position.z = 4.0f;

  mem_set_component(e1, POSITION, pos1);
  mem_set_component(e2, POSITION, pos2);
  mem_set_component(e3, POSITION, pos3);
  mem_set_component(e4, POSITION, pos4);

  struct position p = mem_component(e1, POSITION)->position;
  for (int i = 0; i < 3; i++) {
    assert(fabs(p.vec[i] - pos1.position.vec[i]) < 0.0001);
  };

  // removing item in middle of buffer
  mem_remove_component(e2, POSITION);

  p = mem_component(e1, POSITION)->position;
  for (int i = 0; i < 3; i++) {
    assert(fabs(p.vec[i] - pos1.position.vec[i]) < 0.0001);
  };

  p = mem_component(e4, POSITION)->position;
  for (int i = 0; i < 3; i++) {
    assert(fabs(p.vec[i] - pos4.position.vec[i]) < 0.0001);
  };

  mem_delete_entity(e1);
  mem_delete_entity(e2);
  mem_delete_entity(e3);
  mem_delete_entity(e4);
  assert(mem_count(GEOMETRY_BIT) == 0);
  assert(mem_count(GEOMETRY_BIT | MATERIAL_BIT) == 0);
  assert(mem_count(GEOMETRY_BIT | MATERIAL_BIT | POSITION_BIT) == 0);
  assert(mem_component_count(GEOMETRY) == 0);
  assert(mem_component_count(MATERIAL) == 0);
  assert(mem_component_count(POSITION) == 0);

  mem_create_entity(&e1);
  mem_create_entity(&e2);
  mem_create_entity(&e3);
  mem_create_entity(&e4);

  mem_add_component(e1, GEOMETRY);
  mem_add_component(e2, GEOMETRY);
  mem_add_component(e3, GEOMETRY);

  mem_add_component(e1, MATERIAL);
  mem_add_component(e2, MATERIAL);
  mem_add_component(e3, MATERIAL);

  mem_add_component(e1, POSITION);
  mem_add_component(e2, POSITION);
  mem_add_component(e3, POSITION);
  mem_add_component(e4, POSITION);

  assert(mem_component_count(GEOMETRY) == 3);
  assert(mem_component_count(MATERIAL) == 3);
  assert(mem_component_count(POSITION) == 4);

  assert(mem_count(GEOMETRY_BIT) == 3);
  assert(mem_count(GEOMETRY_BIT | POSITION_BIT) == 3);
  assert(mem_count(GEOMETRY_BIT | MATERIAL_BIT) == 3);
  assert(mem_count(GEOMETRY_BIT | MATERIAL_BIT | POSITION_BIT) == 3);
  assert(mem_count(POSITION_BIT) == 4);

  size_t ne = mem_count(GEOMETRY_BIT | POSITION_BIT);
  uint32_t e_arr[ne];
  mem_entities(GEOMETRY_BIT | POSITION_BIT, e_arr);
  assert(e_arr[0] == e1);
  assert(e_arr[1] == e2);
  assert(e_arr[2] == e3);

  mem_component(e1, GEOMETRY)->geometry.vbo = 1;
  mem_component(e2, GEOMETRY)->geometry.vbo = 2;
  mem_component(e3, GEOMETRY)->geometry.vbo = 3;
  mem_component(e1, POSITION)->position.x = 1.0f;
  mem_component(e2, POSITION)->position.x = 2.0f;
  mem_component(e3, POSITION)->position.x = 3.0f;

  union component arr[ne];
  mem_array(ne, e_arr, GEOMETRY, arr);
  assert(arr[0].geometry.vbo == 1);
  assert(arr[1].geometry.vbo == 2);
  assert(arr[2].geometry.vbo == 3);
  for (size_t i = 0; i < ne; i++) {
    arr[i].geometry.vbo += 5;
  }
  assert(arr[0].geometry.vbo == 6);
  assert(arr[1].geometry.vbo == 7);
  assert(arr[2].geometry.vbo == 8);
  mem_write(ne, e_arr, GEOMETRY, arr);
  assert(mem_component(e1, GEOMETRY)->geometry.vbo == 6);
  assert(mem_component(e2, GEOMETRY)->geometry.vbo == 7);
  assert(mem_component(e3, GEOMETRY)->geometry.vbo == 8);
  TEST_END();
}
