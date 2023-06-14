#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>

#include "gamestate.h"
#include "test.h"

int main() {
  TEST_BEGIN();
  TEST();
  init_mem_subsys();
  uint32_t e1;
  assert(get_identity(0) == ENTITY_UNUSED);
  assert(create_entity(&e1) != -1);
  assert(get_identity(e1) == 0);
  delete_entity(e1);
  assert(get_identity(e1) == 0x80000000);

  create_entity(&e1);
  assert(add_component(e1, MESH) != -1);
  assert(get_identity(e1) == 0x1);

  uint32_t e2, e3, e4;
  create_entity(&e2);
  create_entity(&e3);
  create_entity(&e4);

  assert(add_component(e2, MATERIAL) != -1);

  assert(add_component(e3, MESH) != -1);
  assert(add_component(e3, MATERIAL) != -1);

  assert(add_component(e4, MESH) != -1);
  assert(add_component(e4, MATERIAL) != -1);
  assert(add_component(e4, POSITION) != -1);

  uint64_t mesh_bit = (1LL << MESH);
  uint64_t mat_bit = (1LL << MATERIAL);
  uint64_t pos_bit = (1LL << POSITION);
  

  assert(get_identity(e1) == mesh_bit);
  assert(get_identity(e2) == mat_bit);
  assert(get_identity(e3) == (mat_bit | mesh_bit));
  assert(get_identity(e4) == (mat_bit | mesh_bit | pos_bit));
  assert(get_component_count(MESH) == 3);
  assert(get_component_count(MATERIAL) == 3);
  assert(get_component_count(POSITION) == 1);

  assert(get_num_entities(mesh_bit) == 3);
  assert(get_num_entities(mesh_bit | pos_bit) == 2);
  assert(get_num_entities(mesh_bit | mat_bit | pos_bit) == 1);

  remove_component(e2, MESH);  // should do nothing

  assert(get_identity(e2) == mat_bit);
  remove_component(e4, MATERIAL);
  assert(get_identity(e4) == (mesh_bit | pos_bit));
  add_component(e2, POSITION);
  add_component(e1, POSITION);
  assert(get_identity(e1) == (mesh_bit | pos_bit));
  assert(get_identity(e2) == (mat_bit | pos_bit));
  assert(get_component_count(MESH) == 3);
  assert(get_component_count(MATERIAL) == 2);
  assert(get_component_count(POSITION) == 3);

  add_component(e3, POSITION);
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

  set_component(e1, POSITION, pos1);
  set_component(e2, POSITION, pos2);
  set_component(e3, POSITION, pos3);
  set_component(e4, POSITION, pos4);

  struct position p = get_component(e1, POSITION)->position;
  for (int i = 0; i < 3; i++) {
    assert(fabs(p.elem[i] - pos1.position.elem[i]) < 0.0001);
  };

  // removing item in middle of buffer
  remove_component(e2, POSITION);

  p = get_component(e1, POSITION)->position;
  for (int i = 0; i < 3; i++) {
    assert(fabs(p.elem[i] - pos1.position.elem[i]) < 0.0001);
  };

  p = get_component(e4, POSITION)->position;
  for (int i = 0; i < 3; i++) {
    assert(fabs(p.elem[i] - pos4.position.elem[i]) < 0.0001);
  };

  delete_entity(e1);
  delete_entity(e2);
  delete_entity(e3);
  delete_entity(e4);
  assert(get_num_entities(mesh_bit) == 0);
  assert(get_num_entities(mesh_bit | mat_bit) == 0);
  assert(get_num_entities(mesh_bit | mat_bit | pos_bit) == 0);
  assert(get_component_count(MESH) == 0);
  assert(get_component_count(MATERIAL) == 0);
  assert(get_component_count(POSITION) == 0);

  create_entity(&e1);
  create_entity(&e2);
  create_entity(&e3);
  create_entity(&e4);

  add_component(e1, MESH);
  add_component(e2, MESH);
  add_component(e3, MESH);

  add_component(e1, MATERIAL);
  add_component(e2, MATERIAL);
  add_component(e3, MATERIAL);

  add_component(e1, POSITION);
  add_component(e2, POSITION);
  add_component(e3, POSITION);
  add_component(e4, POSITION);

  assert(get_component_count(MESH) == 3);
  assert(get_component_count(MATERIAL) == 3);
  assert(get_component_count(POSITION) == 4);

  assert(get_num_entities(mesh_bit) == 3);
  assert(get_num_entities(mesh_bit | pos_bit) == 3);
  assert(get_num_entities(mesh_bit | mat_bit) == 3);
  assert(get_num_entities(mesh_bit | mat_bit | pos_bit) == 3);
  assert(get_num_entities(pos_bit) == 4);

  size_t ne = get_num_entities(mesh_bit | pos_bit);
  uint32_t e_arr[ne];
  get_entities(mesh_bit | pos_bit, e_arr);
  assert(e_arr[0] == e1);
  assert(e_arr[1] == e2);
  assert(e_arr[2] == e3);

  get_component(e1, MESH)->mesh.vbo = 1;
  get_component(e2, MESH)->mesh.vbo = 2;
  get_component(e3, MESH)->mesh.vbo = 3;
  get_component(e1, POSITION)->position.x = 1.0f;
  get_component(e2, POSITION)->position.x = 2.0f;
  get_component(e3, POSITION)->position.x = 3.0f;

  union component arr[ne];
  query(ne, e_arr, MESH, arr);
  assert(arr[0].mesh.vbo == 1);
  assert(arr[1].mesh.vbo == 2);
  assert(arr[2].mesh.vbo == 3);
  for (size_t i = 0; i < ne; i++) {
    arr[i].mesh.vbo += 5;
  }
  assert(arr[0].mesh.vbo == 6);
  assert(arr[1].mesh.vbo == 7);
  assert(arr[2].mesh.vbo == 8);
  update(ne, e_arr, MESH, arr);
  assert(get_component(e1, MESH)->mesh.vbo == 6);
  assert(get_component(e2, MESH)->mesh.vbo == 7);
  assert(get_component(e3, MESH)->mesh.vbo == 8);
  TEST_END();
}
