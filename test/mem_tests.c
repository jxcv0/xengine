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
  assert(mem_mask(0) == 0x80000000);
  assert(mem_create_entity(&e1) != -1);
  assert(mem_mask(e1) == 0);
  mem_delete_entity(e1);
  assert(mem_mask(e1) == 0x80000000);

  mem_create_entity(&e1);
  assert(mem_add_component(e1, GEOMETRY) != -1);
  assert(mem_mask(e1) == 0x1);

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

  assert(mem_mask(e1) == GEOMETRY_BIT);
  assert(mem_mask(e2) == MATERIAL_BIT);
  assert(mem_mask(e3) == (GEOMETRY_BIT | MATERIAL_BIT));
  assert(mem_mask(e4) == (GEOMETRY_BIT | MATERIAL_BIT | POSITION_BIT));
  assert(mem_component_count(GEOMETRY) == 3);
  assert(mem_component_count(MATERIAL) == 3);
  assert(mem_component_count(POSITION) == 1);

  assert(mem_count(GEOMETRY_BIT) == 3);
  assert(mem_count(GEOMETRY_BIT | MATERIAL_BIT) == 2);
  assert(mem_count(GEOMETRY_BIT | MATERIAL_BIT | POSITION_BIT) == 1);

  mem_remove_component(e2, GEOMETRY);  // should do nothing

  assert(mem_mask(e2) == MATERIAL_BIT);
  mem_remove_component(e4, MATERIAL);
  assert(mem_mask(e4) == (GEOMETRY_BIT | POSITION_BIT));
  mem_add_component(e2, POSITION);
  mem_add_component(e1, POSITION);
  assert(mem_mask(e1) == (GEOMETRY_BIT | POSITION_BIT));
  assert(mem_mask(e2) == (MATERIAL_BIT | POSITION_BIT));
  assert(mem_component_count(GEOMETRY) == 3);
  assert(mem_component_count(MATERIAL) == 2);
  assert(mem_component_count(POSITION) == 3);

  mem_add_component(e3, POSITION);
  struct position pos1 = {{{1, 1, 1}}};  // this is wild
  struct position pos2 = {{{2, 2, 2}}};
  struct position pos3 = {{{3, 3, 3}}};
  struct position pos4 = {{{4, 4, 4}}};
  mem_component(e1, POSITION)->position = pos1;
  mem_component(e2, POSITION)->position = pos2;
  mem_component(e3, POSITION)->position = pos3;
  mem_component(e4, POSITION)->position = pos4;

  struct position p = mem_component(e1, POSITION)->position;
  for (int i = 0; i < 3; i++) {
    assert(fabs(p.vec[i] - pos1.vec[i]) < 0.0001);
  };

  // removing item in middle of buffer
  mem_remove_component(e2, POSITION);

  p = mem_component(e1, POSITION)->position;
  for (int i = 0; i < 3; i++) {
    assert(fabs(p.vec[i] - pos1.vec[i]) < 0.0001);
  };

  p = mem_component(e4, POSITION)->position;
  for (int i = 0; i < 3; i++) {
    assert(fabs(p.vec[i] - pos4.vec[i]) < 0.0001);
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

  mem_component(e1, GEOMETRY)->geometry.m_vbo = 1;
  mem_component(e2, GEOMETRY)->geometry.m_vbo = 2;
  mem_component(e3, GEOMETRY)->geometry.m_vbo = 3;
  mem_component(e1, POSITION)->position.x = 1.0f;
  mem_component(e2, POSITION)->position.x = 2.0f;
  mem_component(e3, POSITION)->position.x = 3.0f;

  union component arr[ne];
  mem_array(ne, e_arr, GEOMETRY, arr);
  assert(arr[0].geometry.m_vbo == 1);
  assert(arr[1].geometry.m_vbo == 2);
  assert(arr[2].geometry.m_vbo == 3);
  for (size_t i = 0; i < ne; i++) {
    arr[i].geometry.m_vbo += 5;
  }
  assert(arr[0].geometry.m_vbo == 6);
  assert(arr[1].geometry.m_vbo == 7);
  assert(arr[2].geometry.m_vbo == 8);
  mem_write(ne, e_arr, GEOMETRY, arr);
  assert(mem_component(e1, GEOMETRY)->geometry.m_vbo == 6);
  assert(mem_component(e2, GEOMETRY)->geometry.m_vbo == 7);
  assert(mem_component(e3, GEOMETRY)->geometry.m_vbo == 8);
  TEST_END();
}
