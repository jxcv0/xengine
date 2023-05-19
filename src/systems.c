#include "systems.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static uint32_t entity_buf[MAX_NUM_ENTITIES];

/**
 * ----------------------------------------------------------------------------
 */
void sys_update_model_matrices(void) {
  cmpnt_t mask = POSITION_BIT | MODEL_MATRIX_BIT;
  size_t nent = mem_count(mask);
  mem_entities(mask, entity_buf);
}

/**
 * ----------------------------------------------------------------------------
 */
void sys_render_geometries(struct renderer *r, float projection[4][4],
                           float view[4][4]) {
  cmpnt_t mask = GEOMETRY_BIT | MATERIAL_BIT | MODEL_MATRIX_BIT;
  size_t nent = mem_count(mask);
  mem_entities(mask, entity_buf);

  // TODO some kind of persistent (static?) resizable buffer
  union component *geom_buf = malloc(sizeof(union component) * nent);
  union component *mat_buf = malloc(sizeof(union component) * nent);
  union component *model_buf = malloc(sizeof(union component) * nent);

  mem_array(nent, entity_buf, GEOMETRY, geom_buf);
  mem_array(nent, entity_buf, MATERIAL, mat_buf);
  mem_array(nent, entity_buf, MODEL_MATRIX, model_buf);

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

    glBindVertexArray(geom_buf[i].geometry.vao);

    glDrawElements(GL_PATCHES, geom_buf[i].geometry.num_indices,
                   GL_UNSIGNED_INT, 0);
  }

  free(geom_buf);
  free(mat_buf);
}