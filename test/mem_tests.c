#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>

#include "gamestate.h"
#include "test.h"

/*
int main() {
  TEST_BEGIN();
  TEST();
  init_mem_subsys();
  uint32_t e1;
  assert(get_identity(0) == ENTITY_UNUSED);
  assert(create_entity(&e1) != -1);
  assert(get_identity(e1) == 0);
  delete_entity(e1);
  assert(get_identity(e1) == ENTITY_UNUSED);

  create_entity(&e1);
  assert(add_component(e1, component_type_MESH) != -1);
  assert(get_identity(e1) == (1UL << component_type_MESH));

  uint32_t e2, e3, e4;
  create_entity(&e2);
  create_entity(&e3);
  create_entity(&e4);

  assert(add_component(e2, component_type_MATERIAL) != -1);

  assert(add_component(e3, component_type_MESH) != -1);
  assert(add_component(e3, component_type_MATERIAL) != -1);

  assert(add_component(e4, component_type_MESH) != -1);
  assert(add_component(e4, component_type_MATERIAL) != -1);
  assert(add_component(e4, component_type_POSITION) != -1);

  uint64_t mesh_bit = (1LL << component_type_MESH);
  uint64_t mat_bit = (1LL << component_type_MATERIAL);
  uint64_t pos_bit = (1LL << component_type_POSITION);

  uint64_t a[] = {component_type_MESH, component_type_MATERIAL,
                  component_type_POSITION};
  uint64_t m = create_mask(3, a);

  assert(m == (mesh_bit | mat_bit | pos_bit));

  assert(get_identity(e1) == mesh_bit);
  assert(get_identity(e2) == mat_bit);
  assert(get_identity(e3) == (mat_bit | mesh_bit));
  assert(get_identity(e4) == (mat_bit | mesh_bit | pos_bit));
  assert(get_component_count(component_type_MESH) == 3);
  assert(get_component_count(component_type_MATERIAL) == 3);
  assert(get_component_count(component_type_POSITION) == 1);

  assert(get_num_entities(mesh_bit) == 3);
  assert(get_num_entities(mesh_bit | mat_bit) == 2);
  assert(get_num_entities(mesh_bit | mat_bit | pos_bit) == 1);

  remove_component(e2, component_type_MESH);  // should do nothing

  assert(get_identity(e2) == mat_bit);
  remove_component(e4, component_type_MATERIAL);
  assert(get_identity(e4) == (mesh_bit | pos_bit));
  add_component(e2, component_type_POSITION);
  add_component(e1, component_type_POSITION);
  assert(get_identity(e1) == (mesh_bit | pos_bit));
  assert(get_identity(e2) == (mat_bit | pos_bit));
  assert(get_component_count(component_type_MESH) == 3);
  assert(get_component_count(component_type_MATERIAL) == 2);
  assert(get_component_count(component_type_POSITION) == 3);

  add_component(e3, component_type_POSITION);
  union component pos1;
  pos1.as_position.x = 1.0f;
  pos1.as_position.y = 1.0f;
  pos1.as_position.z = 1.0f;

  union component pos2;
  pos2.as_position.x = 2.0f;
  pos2.as_position.y = 2.0f;
  pos2.as_position.z = 2.0f;

  union component pos3;
  pos3.as_position.x = 3.0f;
  pos3.as_position.y = 3.0f;
  pos3.as_position.z = 3.0f;

  union component pos4;
  pos4.as_position.x = 4.0f;
  pos4.as_position.y = 4.0f;
  pos4.as_position.z = 4.0f;

  set_component(e1, component_type_POSITION, pos1);
  set_component(e2, component_type_POSITION, pos2);
  set_component(e3, component_type_POSITION, pos3);
  set_component(e4, component_type_POSITION, pos4);

  vec3_t p = get_component(e1, component_type_POSITION).as_position;
  for (int i = 0; i < 3; i++) {
    assert(fabs(p.elem[i] - pos1.as_position.elem[i]) < 0.0001);
  };

  // removing item in middle of buffer
  remove_component(e2, component_type_POSITION);

  p = get_component(e1, component_type_POSITION).as_position;
  for (int i = 0; i < 3; i++) {
    assert(fabs(p.elem[i] - pos1.as_position.elem[i]) < 0.0001);
  };

  p = get_component(e4, component_type_POSITION).as_position;
  for (int i = 0; i < 3; i++) {
    assert(fabs(p.elem[i] - pos4.as_position.elem[i]) < 0.0001);
  };

  delete_entity(e1);
  delete_entity(e2);
  delete_entity(e3);
  delete_entity(e4);
  assert(get_num_entities(mesh_bit) == 0);
  assert(get_num_entities(mesh_bit | mat_bit) == 0);
  assert(get_num_entities(mesh_bit | mat_bit | pos_bit) == 0);
  assert(get_component_count(component_type_MESH) == 0);
  assert(get_component_count(component_type_MATERIAL) == 0);
  assert(get_component_count(component_type_POSITION) == 0);

  create_entity(&e1);
  create_entity(&e2);
  create_entity(&e3);
  create_entity(&e4);

  add_component(e1, component_type_MESH);
  add_component(e2, component_type_MESH);
  add_component(e3, component_type_MESH);

  add_component(e1, component_type_MATERIAL);
  add_component(e2, component_type_MATERIAL);
  add_component(e3, component_type_MATERIAL);

  add_component(e1, component_type_POSITION);
  add_component(e2, component_type_POSITION);
  add_component(e3, component_type_POSITION);
  add_component(e4, component_type_POSITION);

  assert(get_component_count(component_type_MESH) == 3);
  assert(get_component_count(component_type_MATERIAL) == 3);
  assert(get_component_count(component_type_POSITION) == 4);

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

  cmpnt_t c;
  c = get_component(e1, component_type_MESH);
  c.as_mesh.vbo = 1;
  set_component(e1, component_type_MESH, c);

  c = get_component(e2, component_type_MESH);
  c.as_mesh.vbo = 2;
  set_component(e2, component_type_MESH, c);

  c = get_component(e3, component_type_MESH);
  c.as_mesh.vbo = 3;
  set_component(e3, component_type_MESH, c);

  c = get_component(e1, component_type_POSITION);
  c.as_position.x = 1.0f;
  set_component(e1, component_type_POSITION, c);

  c = get_component(e2, component_type_POSITION);
  c.as_position.x = 2.0f;
  set_component(e2, component_type_POSITION, c);

  c = get_component(e3, component_type_POSITION);
  c.as_position.x = 3.0f;
  set_component(e3, component_type_POSITION, c);

  union component arr[ne];
  query(ne, e_arr, component_type_MESH, arr);
  assert(arr[0].as_mesh.vbo == 1);
  assert(arr[1].as_mesh.vbo == 2);
  assert(arr[2].as_mesh.vbo == 3);
  for (size_t i = 0; i < ne; i++) {
    arr[i].as_mesh.vbo += 5;
  }
  assert(arr[0].as_mesh.vbo == 6);
  assert(arr[1].as_mesh.vbo == 7);
  assert(arr[2].as_mesh.vbo == 8);
  update(ne, e_arr, component_type_MESH, arr);
  assert(get_component(e1, component_type_MESH).as_mesh.vbo == 6);
  assert(get_component(e2, component_type_MESH).as_mesh.vbo == 7);
  assert(get_component(e3, component_type_MESH).as_mesh.vbo == 8);
  TEST_END();
}
*/

int test_init_entity_table(void) {
  gamestate_t gs = create_gamestate(16);
  for (int i = 0; i < 16; i++) {
    if (gs.otable[i] != 0) {
      return 1;
    }
  }

  if (gs.nobj != 0) {
    return 1;
  }

  if (gs.max_nobj != 16) {
    return 1;
  }

  return 0;
}

int test_create_mask(void) {
  uint64_t mesh_bit = (1LL << component_type_MESH);
  uint64_t mat_bit = (1LL << component_type_MATERIAL);
  uint64_t pos_bit = (1LL << component_type_POSITION);

  uint64_t a[] = {component_type_MESH, component_type_MATERIAL,
                  component_type_POSITION};
  uint64_t m = create_mask(3, a);

  if (m == (mesh_bit | mat_bit | pos_bit)) {
    return 0;
  }
  return 1;
}

int test_create_game_object(void) {
  gamestate_t gs = create_gamestate(16);

  for (int i = 0; i < 15; i++) {
    if (create_game_object(&gs) == -1) {
      return 1;
    }
  }

  if (create_game_object(&gs) == 0) {
    return 1;
  }

  return 0;
}

int main() {
  int err = 0;
  err += test_init_entity_table();
  err += test_create_mask();
  err += test_create_game_object();
  return err;
}