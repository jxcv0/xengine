#include "render.h"

#include <assert.h>
#include <stdio.h>

#include "glad.h"
#include "light.h"
#include "mesh.h"
#include "shader.h"

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

  gen_material_buffers(&mesh->m_material);
}

/**
 * ----------------------------------------------------------------------------
 */
static void gen_texture_buffers(struct texture *texture) {
  if (texture->mp_data != NULL) {
    glGenTextures(1, &texture->m_texture_id);
    glBindTexture(GL_TEXTURE_2D, texture->m_texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    int format;
    switch (texture->m_num_channels) {
      case 1:
        format = GL_RED;
        break;
      case 4:
        format = GL_RGBA;
        break;
      default:
        format = GL_RGB;
        break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, texture->m_width, texture->m_height,
                 0, format, GL_UNSIGNED_BYTE, texture->mp_data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void gen_material_buffers(struct material *material) {
  gen_texture_buffers(&material->m_tex_diffuse);
  // gen_texture_buffers(&material->m_tex_normal);
  // gen_texture_buffers(&material->m_tex_specular);
}

/**
 * ----------------------------------------------------------------------------
 */
void draw_mesh(const shader_t shader, const mat4 projection_matrix,
               const mat4 view_matrix, const mat4 model_matrix,
               struct light *light, const struct mesh *mesh) {
  glUseProgram(shader);

  // TODO init the shader on creation with projection matrix, it should never
  // change.
  // Also do these need to be set each time?
  shader_set_uniform_m4fv(shader, "projection", projection_matrix);
  shader_set_uniform_m4fv(shader, "model", model_matrix);
  shader_set_uniform_m4fv(shader, "view", view_matrix);

  // TODO best way to handle lights?
  shader_set_uniform_3fv(shader, "light_pos", light->m_position);
  shader_set_uniform_3fv(shader, "light_color", light->m_color);

  shader_set_uniform_3fv(shader, "m_diffuse_color",
                         mesh->m_material.m_diffuse_color);

  shader_set_uniform_1i(shader, "diffuse_texture", 0);  // same as active tex

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mesh->m_material.m_tex_diffuse.m_texture_id);

  glBindVertexArray(mesh->m_vao);
  glDrawArrays(GL_TRIANGLES, 0, mesh->m_num_vertices);
}
