#include "mesh.h"

#include <glad.h>
#include <stdbool.h>
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
static ssize_t find_char(const char c, const size_t pos, const char *s,
                         const size_t len) {
  for (size_t i = pos; i < len; i++) {
    if (s[i] == c) {
      return i;
    }
  }
  return -1;
}

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
 */
void parse_index(struct index *index, const char *line, const size_t len) {
  (void)index;
  ssize_t curr = 0;
  ssize_t prev = 0;
  while ((curr = find_char(' ', prev, line, len)) != -1) {
    ssize_t new_len = curr - prev;
    char str[new_len + 1];
    strncpy(str, &line[len - new_len], new_len);
    str[new_len] = '\0';
    printf("%s\n", str);
    prev = curr + 1;
  }
}

/**
 * ----------------------------------------------------------------------------
 */
static ssize_t get_line(const struct mmapfile *file, const ssize_t pos) {
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
  while ((curr = get_line(&file, prev)) != -1) {
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
  struct index indices[f_count * 3];
  struct vertex vertices[f_count * 3];

  (void)vertices;

  v_count = 0;
  vt_count = 0;
  vn_count = 0;
  f_count = 0;

  curr = 0;
  prev = 0;
  while ((curr = get_line(&file, prev)) != -1) {
    char *lineptr = &((char *)file.mp_addr)[prev];
    if (strncmp(lineptr, "v ", 2) == 0) {
      parse_vec3(positions[v_count++], &lineptr[2]);

    } else if (strncmp(lineptr, "vt ", 3) == 0) {
      parse_vec2(tex_coords[vt_count++], &lineptr[3]);

    } else if (strncmp(lineptr, "vn ", 3) == 0) {
      parse_vec3(normals[vn_count++], &lineptr[3]);

    } else if (strncmp(lineptr, "f ", 2) == 0) {
      size_t len = curr - prev;
      parse_index(indices, &lineptr[2], len);
      f_count += 3;  // 3 faces per line
    }
    prev = curr + 1;
  }

  /*
  for (size_t i = 0; i < v_count; i++) {
    printf("v %f, %f, %f\n", positions[i][0], positions[i][1], positions[i][2]);
  }

  for (size_t i = 0; i < vt_count; i++) {
    printf("vt %f, %f\n", tex_coords[i][0], tex_coords[i][1]);
  }

  for (size_t i = 0; i < vn_count; i++) {
    printf("vn %f, %f, %f\n", normals[i][0], normals[i][1], normals[i][2]);
  }
  */

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
