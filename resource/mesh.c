#include "mesh.h"

#include <glad.h>
#include <immintrin.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mmapfile.h"

typedef float vec3[3];
typedef float vec2[2];

struct index {
  unsigned int m_position_idx;
  unsigned int m_tex_coord_idx;
  unsigned int m_normal_idx;
};

/**
 * ----------------------------------------------------------------------------
 */
void parse_vec3(float *v, const char *line) {
  const char *p = line;
  char *end;
  v[0] = strtof(p, &end);
  p = end;
  v[1] = strtof(p, &end);
  p = end;
  v[2] = strtof(p, &end);
}

/**
 * ----------------------------------------------------------------------------
 */
void parse_vec2(float *v, const char *line) {
  const char *p = line;
  char *end;
  v[0] = strtof(p, &end);
  p = end;
  v[1] = strtof(p, &end);
}

/**
 * ----------------------------------------------------------------------------
Mesh::Index parse_index(const std::string_view &sv) {
  using size_type = std::string_view::size_type;
  size_type curr = 0;
  size_type prev = 0;

  Mesh::Index index;
  curr = sv.find('/', prev);
  auto len = curr - prev;
  auto pos_index = sv.substr(prev, len);
  index.m_position_idx = std::atoi(pos_index.data()) - 1;
  prev += len + 1;

  curr = sv.find('/', prev);
  len = curr - prev;
  auto tex_coords_index = sv.substr(prev, len);
  index.m_tex_coord_idx = std::atoi(tex_coords_index.data()) - 1;
  prev += len + 1;

  curr = sv.find('/', sv.size() - prev);
  len = curr - prev;
  auto normal_index = sv.substr(prev, len);
  index.m_normal_idx = std::atoi(normal_index.data()) - 1;

  return index;
}
 */

static ssize_t next_token(const struct mmapfile *file, const ssize_t pos) {
  for (size_t i = pos; i < file->m_size; i++) {
    char c = ((char *)file->mp_addr)[i];
    if (c == '\n') {
      return i;
    }
  }
  return -1;
}

/**
 * ----------------------------------------------------------------------------
 */
struct mesh mesh_load(const char *filepath) {
  struct mmapfile file = mmapfile_map(filepath);
  struct mesh mesh = {0};

  size_t v_count = 0;
  size_t vt_count = 0;
  size_t vn_count = 0;
  size_t f_count = 0;

  // count up how much memory to allocate.
  ssize_t curr = 0;
  ssize_t prev = 0;
  while ((curr = next_token(&file, prev)) != -1) {
    char *lineptr = &((char *)file.mp_addr)[prev];
    if (strncmp(lineptr, "v ", 2) == 0) {
      v_count++;
    } else if (strncmp(lineptr, "vt ", 3) == 0) {
      vt_count++;
    } else if (strncmp(lineptr, "vn ", 3) == 0) {
      vn_count++;
    } else if (strncmp(lineptr, "f ", 2) == 0) {
      f_count += 3;  // 3 faces per line
    }
    prev = curr + 1;
  }

  vec3 positions[v_count];
  vec2 tex_coords[vt_count];
  vec3 normals[vn_count];
  struct vertex vertices[f_count * 3];

  (void)vertices;

  v_count = 0;
  vt_count = 0;
  vn_count = 0;
  f_count = 0;

  curr = 0;
  prev = 0;
  while ((curr = next_token(&file, prev)) != -1) {
    // size_t len = curr - prev;
    char *lineptr = &((char *)file.mp_addr)[prev];
    if (strncmp(lineptr, "v ", 2) == 0) {
      parse_vec3(positions[v_count++], &lineptr[2]);

    } else if (strncmp(lineptr, "vt ", 3) == 0) {
      parse_vec2(tex_coords[vt_count++], &lineptr[3]);

    } else if (strncmp(lineptr, "vn ", 3) == 0) {
      parse_vec3(normals[vn_count++], &lineptr[3]);

    } else if (strncmp(lineptr, "f ", 2) == 0) {
      f_count += 3;  // 3 faces per line
    }
    prev = curr + 1;
  }

  for (size_t i = 0; i < v_count; i++) {
    printf("v %f, %f, %f\n", positions[i][0], positions[i][1], positions[i][2]);
  }

  for (size_t i = 0; i < vt_count; i++) {
    printf("vt %f, %f\n", tex_coords[i][0], tex_coords[i][1]);
  }

  for (size_t i = 0; i < vn_count; i++) {
    printf("vn %f, %f, %f\n", normals[i][0], normals[i][1], normals[i][2]);
  }

  /**
  for (unsigned int i = 0; i < mesh.m_num_vertices; i++) {
    auto index = indices[i];
    mesh.mp_vertices[i].m_position = positions[index.m_position_idx];
    mesh.mp_vertices[i].m_normal = normals[index.m_normal_idx];
    mesh.mp_vertices[i].m_tex_coord = tex_coords[index.m_tex_coord_idx];
  }

  */
  mmapfile_unmap(&file);
  return mesh;
}

/**
 * ----------------------------------------------------------------------------
 */
void mesh_unload(struct mesh *mesh) {
  (void)mesh;
  // free(mesh->mp_vertices);
  // mesh->mp_vertices = NULL;
}

/**
 * ----------------------------------------------------------------------------
 */
void gen_mesh_buffers(struct mesh *mesh) {
  (void)mesh;
  /*
glGenBuffers(1, &mesh->m_vbo);
// glGenBuffers(1, &m_ebo);
glGenVertexArrays(1, &mesh->m_vao);
glBindVertexArray(mesh->m_vao);

glBindBuffer(GL_ARRAY_BUFFER, mesh->m_vbo);
glBufferData(GL_ARRAY_BUFFER, mesh->m_num_vertices * sizeof(struct vertex),
             (void *)(mesh->mp_vertices), GL_DYNAMIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                      (void *)(0));
glEnableVertexAttribArray(0);

glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                      (void *)(offsetof(struct vertex, m_normal)));
glEnableVertexAttribArray(1);

glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                      (void *)(offsetof(struct vertex, m_tex_coord)));
glEnableVertexAttribArray(2);
*/
}

/**
 * ----------------------------------------------------------------------------
 */
void draw_mesh(struct mesh *mesh) {
  glBindVertexArray(mesh->m_vao);
  glDrawArrays(GL_TRIANGLES, 0, mesh->m_num_vertices);
  /*
  glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_INT,
                 static_cast<void *>(0));
  */
}
