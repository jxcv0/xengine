#include "systems.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_NUM_THREADS 8

static uint32_t entity_buf[MAX_NUM_ENTITIES];

// TODO This buffer is enormous
static union component component_buffers[4][MAX_NUM_ENTITIES];

/**
 * ----------------------------------------------------------------------------
 */
void sys_update_model_matrices(void) {
  uint64_t components[] = {POSITION, MODEL_MATRIX};
  uint64_t mask = create_mask(2, components);
  size_t nent = get_num_entities(mask);
  get_entities(mask, entity_buf);

  union component *pos_buf = component_buffers[0];
  union component *model_buf = component_buffers[1];

  query(nent, entity_buf, POSITION, pos_buf);
  query(nent, entity_buf, MODEL_MATRIX, model_buf);

#pragma omp parallel for num_threads(MAX_NUM_THREADS) schedule(static)
  for (size_t i = 0; i < nent; i++) {
    translate(model_buf[i].model_matrix.elem, pos_buf[i].position.elem);
  }

  update(nent, entity_buf, POSITION, pos_buf);
  update(nent, entity_buf, MODEL_MATRIX, model_buf);
}

/**
 * ----------------------------------------------------------------------------
 */
void sys_load(uint64_t component_type) {
  uint64_t result_type;
  switch (component_type) {
    case MESH_LOAD_REQUEST:
      result_type = MESH;
      break;
    case MAT_LOAD_REQUEST:
      result_type = MATERIAL;
      break;
    default:
      return;
  }
  uint64_t mask = (1 << component_type);
  size_t nent = get_num_entities(mask);
  get_entities(mask, entity_buf);

  union component *buf = component_buffers[0];
  query(nent, entity_buf, component_type, buf);
#pragma omp parallel for num_threads(MAX_NUM_THREADS) schedule(static)
  for (size_t i = 0; i < nent; i++) {
    union component comp;
    switch (result_type) {
      case MESH:
        if (load_obj(&comp.mesh, buf[i].request.path) != 0) {
          continue;
        }
        break;

      case MATERIAL:
        if (load_mtl(&comp.material, buf[i].request.path) != 0) {
          continue;
        }
        break;
    }
    add_component(entity_buf[i], result_type);
    remove_component(entity_buf[i], component_type);
    set_component(entity_buf[i], result_type, comp);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void sys_render_geometries(struct renderer *r, float projection[4][4],
                           float view[4][4]) {
  uint64_t components[] = {MESH, MATERIAL, MODEL_MATRIX};
  uint64_t mask = create_mask(3, components);
  size_t nent = get_num_entities(mask);
  get_entities(mask, entity_buf);

  union component *mesh_buf = component_buffers[1];
  union component *mat_buf = component_buffers[2];
  union component *model_buf = component_buffers[3];

  query(nent, entity_buf, MESH, mesh_buf);
  query(nent, entity_buf, MATERIAL, mat_buf);
  query(nent, entity_buf, MODEL_MATRIX, model_buf);

  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, r->g_buff);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(r->deferred_geometry);
  shader_set_uniform_m4fv(r->deferred_geometry, "projection", projection);
  shader_set_uniform_m4fv(r->deferred_geometry, "view", view);

  for (size_t i = 0; i < nent; i++) {
    shader_set_uniform_m4fv(r->deferred_geometry, "model",
                            model_buf[i].model_matrix.elem);
    shader_set_uniform_1i(r->deferred_geometry, "tex_diffuse", 0);
    shader_set_uniform_1i(r->deferred_geometry, "tex_roughness", 1);
    shader_set_uniform_1i(r->deferred_geometry, "tex_normal", 2);
    shader_set_uniform_1i(r->deferred_geometry, "tex_metallic", 3);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mat_buf[i].material.diffuse);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mat_buf[i].material.roughness);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mat_buf[i].material.normal);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, mat_buf[i].material.metallic);

    glBindVertexArray(mesh_buf[i].mesh.vao);

    glDrawElements(GL_TRIANGLES, mesh_buf[i].mesh.num_indices, GL_UNSIGNED_INT,
                   0);
  }
}
