#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>

#include "gamestate.h"
#include "test.h"

int test_static_gamestate(void) {
  int err = 0;
  init_gamestate();
  uint32_t e1;
  if (get_attribs(0) != ENTITY_UNUSED) {
    ++err;
  }

  if (create_obj(&e1) == -1) {
    ++err;
  }

  if (get_attribs(e1) != 0) {
    ++err;
  }

  delete_obj(e1);
  if (get_attribs(e1) != ENTITY_UNUSED) {
    ++err;
  }

  create_obj(&e1);
  if (add_attrib(e1, attrib_type_MESH) == -1) {
    ++err;
  }

  if (get_attribs(e1) != (1UL << attrib_type_MESH)) {
    ++err;
  }

  uint32_t e2, e3, e4;
  create_obj(&e2);
  create_obj(&e3);
  create_obj(&e4);

  if (add_attrib(e2, attrib_type_MATERIAL) == -1) {
    ++err;
  }

  if (add_attrib(e3, attrib_type_MESH) == -1) {
    ++err;
  }

  if (add_attrib(e3, attrib_type_MATERIAL) == -1) {
    ++err;
  }

  if (add_attrib(e4, attrib_type_MESH) == -1) {
    ++err;
  }

  if (add_attrib(e4, attrib_type_MATERIAL) == -1) {
    ++err;
  }

  if (add_attrib(e4, attrib_type_POSITION) == -1) {
    ++err;
  }

  uint64_t mesh_bit = (1LL << attrib_type_MESH);
  uint64_t mat_bit = (1LL << attrib_type_MATERIAL);
  uint64_t pos_bit = (1LL << attrib_type_POSITION);

  if (get_attribs(e1) != mesh_bit) {
    ++err;
  }

  if (get_attribs(e2) != mat_bit) {
    ++err;
  }

  if (get_attribs(e3) != (mat_bit | mesh_bit)) {
    ++err;
  }

  if (get_attribs(e4) != (mat_bit | mesh_bit | pos_bit)) {
    ++err;
  }

  if (get_num_attribs(attrib_type_MESH) != 3) {
    ++err;
  }

  if (get_num_attribs(attrib_type_MATERIAL) != 3) {
    ++err;
  }

  if (get_num_attribs(attrib_type_POSITION) != 1) {
    ++err;
  }

  if (get_num_obj(mesh_bit) != 3) {
  }

  if (get_num_obj(mesh_bit | mat_bit) != 2) {
    ++err;
  }

  if (get_num_obj(mesh_bit | mat_bit | pos_bit) != 1) {
    ++err;
  }

  remove_attrib(e2, attrib_type_MESH);  // should do nothing

  if (get_attribs(e2) != mat_bit) {
    ++err;
  }

  remove_attrib(e4, attrib_type_MATERIAL);

  if (get_attribs(e4) != (mesh_bit | pos_bit)) {
    ++err;
  }

  add_attrib(e2, attrib_type_POSITION);
  add_attrib(e1, attrib_type_POSITION);

  if (get_attribs(e1) != (mesh_bit | pos_bit)) {
    ++err;
  }

  if (get_attribs(e2) != (mat_bit | pos_bit)) {
    ++err;
  }

  if (get_num_attribs(attrib_type_MESH) != 3) {
    ++err;
  }

  if (get_num_attribs(attrib_type_MATERIAL) != 2) {
    ++err;
  }

  if (get_num_attribs(attrib_type_POSITION) != 3) {
    ++err;
  }

  add_attrib(e3, attrib_type_POSITION);
  union attribute pos1;
  pos1.as_position.x = 1.0f;
  pos1.as_position.y = 1.0f;
  pos1.as_position.z = 1.0f;

  union attribute pos2;
  pos2.as_position.x = 2.0f;
  pos2.as_position.y = 2.0f;
  pos2.as_position.z = 2.0f;

  union attribute pos3;
  pos3.as_position.x = 3.0f;
  pos3.as_position.y = 3.0f;
  pos3.as_position.z = 3.0f;

  union attribute pos4;
  pos4.as_position.x = 4.0f;
  pos4.as_position.y = 4.0f;
  pos4.as_position.z = 4.0f;

  set_attrib(e1, attrib_type_POSITION, pos1);
  set_attrib(e2, attrib_type_POSITION, pos2);
  set_attrib(e3, attrib_type_POSITION, pos3);
  set_attrib(e4, attrib_type_POSITION, pos4);

  vec3_t p = get_attrib(e1, attrib_type_POSITION).as_position;
  for (int i = 0; i < 3; i++) {
    if (fabs(p.elem[i] - pos1.as_position.elem[i]) > 0.0001) {
      ++err;
    }
  };

  // removing item in middle of buffer
  remove_attrib(e2, attrib_type_POSITION);

  p = get_attrib(e1, attrib_type_POSITION).as_position;
  for (int i = 0; i < 3; i++) {
    if (fabs(p.elem[i] - pos1.as_position.elem[i]) > 0.0001) {
      ++err;
    }
  };

  p = get_attrib(e4, attrib_type_POSITION).as_position;
  for (int i = 0; i < 3; i++) {
    if (fabs(p.elem[i] - pos4.as_position.elem[i]) < 0.0001) {
    }
  };

  delete_obj(e1);
  delete_obj(e2);
  delete_obj(e3);
  delete_obj(e4);
  if (get_num_obj(mesh_bit) != 0) {
    ++err;
  }

  if (get_num_obj(mesh_bit | mat_bit) != 0) {
    ++err;
  }

  if (get_num_obj(mesh_bit | mat_bit | pos_bit) != 0) {
    ++err;
  }

  if (get_num_attribs(attrib_type_MESH) != 0) {
    ++err;
  }

  if (get_num_attribs(attrib_type_MATERIAL) != 0) {
    ++err;
  }

  if (get_num_attribs(attrib_type_POSITION) != 0) {
    ++err;
  }

  create_obj(&e1);
  create_obj(&e2);
  create_obj(&e3);
  create_obj(&e4);

  add_attrib(e1, attrib_type_MESH);
  add_attrib(e2, attrib_type_MESH);
  add_attrib(e3, attrib_type_MESH);

  add_attrib(e1, attrib_type_MATERIAL);
  add_attrib(e2, attrib_type_MATERIAL);
  add_attrib(e3, attrib_type_MATERIAL);

  add_attrib(e1, attrib_type_POSITION);
  add_attrib(e2, attrib_type_POSITION);
  add_attrib(e3, attrib_type_POSITION);
  add_attrib(e4, attrib_type_POSITION);

  if (get_num_attribs(attrib_type_MESH) != 3) {
    ++err;
  }

  if (get_num_attribs(attrib_type_MATERIAL) != 3) {
    ++err;
  }

  if (get_num_attribs(attrib_type_POSITION) != 4) {
    ++err;
  }

  if (get_num_obj(mesh_bit) != 3) {
    ++err;
  }

  if (get_num_obj(mesh_bit | pos_bit) != 3) {
    ++err;
  }

  if (get_num_obj(mesh_bit | mat_bit) != 3) {
    ++err;
  }

  if (get_num_obj(mesh_bit | mat_bit | pos_bit) != 3) {
    ++err;
  }

  if (get_num_obj(pos_bit) != 4) {
    ++err;
  }

  size_t ne = get_num_obj(mesh_bit | pos_bit);
  uint32_t e_arr[ne];
  get_objs(mesh_bit | pos_bit, e_arr);

  if (e_arr[0] != e1) {
    ++err;
  }

  if (e_arr[1] != e2) {
    ++err;
  }

  if (e_arr[2] != e3) {
    ++err;
  }

  attrib_t c;
  c = get_attrib(e1, attrib_type_MESH);
  c.as_mesh.vbo = 1;
  set_attrib(e1, attrib_type_MESH, c);

  c = get_attrib(e2, attrib_type_MESH);
  c.as_mesh.vbo = 2;
  set_attrib(e2, attrib_type_MESH, c);

  c = get_attrib(e3, attrib_type_MESH);
  c.as_mesh.vbo = 3;
  set_attrib(e3, attrib_type_MESH, c);

  c = get_attrib(e1, attrib_type_POSITION);
  c.as_position.x = 1.0f;
  set_attrib(e1, attrib_type_POSITION, c);

  c = get_attrib(e2, attrib_type_POSITION);
  c.as_position.x = 2.0f;
  set_attrib(e2, attrib_type_POSITION, c);

  c = get_attrib(e3, attrib_type_POSITION);
  c.as_position.x = 3.0f;
  set_attrib(e3, attrib_type_POSITION, c);

  union attribute arr[ne];
  query(ne, e_arr, attrib_type_MESH, arr);

  if (arr[0].as_mesh.vbo != 1) {
    ++err;
  }

  if (arr[1].as_mesh.vbo != 2) {
    ++err;
  }

  if (arr[2].as_mesh.vbo != 3) {
    ++err;
  }

  for (size_t i = 0; i < ne; i++) {
    arr[i].as_mesh.vbo += 5;
  }

  if (arr[0].as_mesh.vbo != 6) {
    ++err;
  }

  if (arr[1].as_mesh.vbo != 7) {
    ++err;
  }

  if (arr[2].as_mesh.vbo != 8) {
    ++err;
  }

  update(ne, e_arr, attrib_type_MESH, arr);

  if (get_attrib(e1, attrib_type_MESH).as_mesh.vbo != 6) {
    ++err;
  }

  if (get_attrib(e2, attrib_type_MESH).as_mesh.vbo != 7) {
    ++err;
  }

  if (get_attrib(e3, attrib_type_MESH).as_mesh.vbo != 8) {
    ++err;
  }

  return err;
}

int main() {
  test_static_gamestate();
  TEST_END();
  return 0;
}
