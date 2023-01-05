#include "render.h"

#include "glad.h"

/**
 * ----------------------------------------------------------------------------
 */
void mesh_buffer(struct mesh *mesh) {
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
void mesh_draw(struct mesh *mesh) {
  glBindVertexArray(mesh->m_vao);
  glDrawArrays(GL_TRIANGLES, 0, mesh->m_num_vertices);
  /*
  glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_INT,
                 static_cast<void *>(0));
  */
}
