#include "systems.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM_THREADS 8

static uint32_t entity_buf[MAX_NUM_ENTITIES];

// TODO This buffer is enormous
static union attribute component_buffers[4][MAX_NUM_ENTITIES];

/**
 * ----------------------------------------------------------------------------
 */
void sys_update_model_matrices(void) {
  uint64_t components[] = {attrib_type_POSITION, attrib_type_ROTATION,
                           attrib_type_MODEL_MATRIX};
  uint64_t mask = create_mask(3, components);
  size_t nent = get_num_obj(mask);
  get_objs(mask, entity_buf);

  union attribute *pos_buf = component_buffers[0];
  union attribute *rot_buf = component_buffers[1];
  union attribute *model_buf = component_buffers[2];

  query(nent, entity_buf, attrib_type_POSITION, pos_buf);
  query(nent, entity_buf, attrib_type_ROTATION, rot_buf);
  query(nent, entity_buf, attrib_type_MODEL_MATRIX, model_buf);

#pragma omp parallel for num_threads(MAX_NUM_THREADS) schedule(static)
  for (size_t i = 0; i < nent; i++) {
    vec3_t pos = pos_buf[i].as_position;
    struct rotation rot = rot_buf[i].as_rotation;
    mat4_t model = model_buf[i].as_model_matrix;

    model = rotate(model, rot.axis, rot.radians);
    model = translate(model, pos);
    model_buf[i].as_model_matrix = model;
  }

  update(nent, entity_buf, attrib_type_POSITION, pos_buf);
  update(nent, entity_buf, attrib_type_ROTATION, rot_buf);
  update(nent, entity_buf, attrib_type_MODEL_MATRIX, model_buf);
}

/**
 * ----------------------------------------------------------------------------
 */
void sys_load(uint64_t attrib_type) {
  uint64_t result_type;
  switch (attrib_type) {
    case attrib_type_MESH_LOAD_REQUEST:
      result_type = attrib_type_MESH;
      break;
    case attrib_type_MAT_LOAD_REQUEST:
      result_type = attrib_type_MATERIAL;
      break;
    default:
      return;
  }
  uint64_t mask = (1 << attrib_type);
  size_t nent = get_num_obj(mask);
  get_objs(mask, entity_buf);

  union attribute *buf = component_buffers[0];
  query(nent, entity_buf, attrib_type, buf);

  for (size_t i = 0; i < nent; i++) {
    union attribute comp;
    switch (result_type) {
      case attrib_type_MESH:
        if (load_mesh(&comp.as_mesh, buf[i].as_request.path) != 0) {
          fprintf(stderr, "Unable to load asset: %s", buf[i].as_request.path);
        }
        break;

      case attrib_type_MATERIAL:
        if (load_mtl(&comp.as_material, buf[i].as_request.path) != 0) {
          fprintf(stderr, "Unable to load asset: %s", buf[i].as_request.path);
        }
        break;
    }
    add_attrib(entity_buf[i], result_type);
    remove_attrib(entity_buf[i], attrib_type);
    set_attrib(entity_buf[i], result_type, comp);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void sys_render_geometries(struct renderer *r, mat4_t projection, mat4_t view) {
  uint64_t components[] = {attrib_type_MESH, attrib_type_MATERIAL,
                           attrib_type_MODEL_MATRIX};
  uint64_t mask = create_mask(3, components);
  size_t nent = get_num_obj(mask);
  get_objs(mask, entity_buf);

  union attribute *mesh_buf = component_buffers[1];
  union attribute *mat_buf = component_buffers[2];
  union attribute *model_buf = component_buffers[3];

  query(nent, entity_buf, attrib_type_MESH, mesh_buf);
  query(nent, entity_buf, attrib_type_MATERIAL, mat_buf);
  query(nent, entity_buf, attrib_type_MODEL_MATRIX, model_buf);

  prepare_gbuf(r, projection.elem, view.elem);

  for (size_t i = 0; i < nent; i++) {
    render_geom_to_gbuf(r, &mat_buf[i].as_material, &mesh_buf[i].as_mesh,
                        &model_buf[i].as_model_matrix.elem);
  }
  // glBindTexture(GL_TEXTURE_2D, 0);
  // glBindVertexArray(0);
}
