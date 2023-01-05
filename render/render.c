#include "render.h"

#include "glad.h"
#include "light.h"

/**
 * ----------------------------------------------------------------------------
 */
void gen_mesh_buffers(struct mesh *mesh) {
  glGenBuffers(1, &mesh->m_vbo);
  glGenVertexArrays(1, &mesh->m_vao);
  glBindVertexArray(mesh->m_vao);

  glBindBuffer(GL_ARRAY_BUFFER, mesh->m_vbo);
  glBufferData(GL_ARRAY_BUFFER, mesh->m_num_vertices * sizeof(struct vertex),
               (void *)(mesh->mp_vertices), GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(offsetof(struct vertex, m_position)));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(offsetof(struct vertex, m_tex_coord)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(offsetof(struct vertex, m_normal)));
  glEnableVertexAttribArray(2);
}

/**
 * ----------------------------------------------------------------------------
 */
void draw_mesh(const shader_t shader, const mat4 projection_matrix,
               const mat4 view_matrix, const mat4 model_matrix,
               struct light *light, const struct mesh *mesh) {
  glUseProgram(shader);

  // TODO init the shader on creation with projection matrix, it should never
  // change
  shader_set_uniform_m4fv(shader, "projection", projection_matrix);
  shader_set_uniform_m4fv(shader, "model", model_matrix);
  shader_set_uniform_m4fv(shader, "view", view_matrix);

  // TODO best way to handle lights?
  shader_set_uniform_3fv(shader, "light_pos", light->m_position);
  shader_set_uniform_3fv(shader, "light_color", light->m_color);

  // TODO this needs to be set with mtl data
  // mesh->mp_material.m_color
  vec3 mesh_color = {0.3, 0.5, 0.1};  // this is temporary
  shader_set_uniform_3fv(shader, "obj_color", mesh_color);

  glBindVertexArray(mesh->m_vao);
  glDrawArrays(GL_TRIANGLES, 0, mesh->m_num_vertices);
}
