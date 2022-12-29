#include "mesh.h"

#include <glad.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mmapfile.h"

struct index {
  size_t m_position_idx;
  size_t m_tex_coord_idx;
  size_t m_normal_idx;
};
/**
 * ----------------------------------------------------------------------------
 */
static ssize_t find_delim(const char c, const size_t pos, const char *s,
                          const size_t len) {
  for (size_t i = pos; i < len; i++) {
    char ch = s[i];
    if (ch == c || ch == '\n') {
      return i;
    }
  }
  return -1;
}

/**
 * ----------------------------------------------------------------------------
 */
static void parse_vec3(float *v, const char *line) {
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
static void parse_vec2(float *v, const char *line) {
  const char *p = line;
  char *end;
  v[0] = strtof(p, &end);
  p = end;
  v[1] = strtof(p, &end);
}

/**
 * ----------------------------------------------------------------------------
 */
static size_t strntoul(const char *str, size_t n) {
  size_t res = 0;
  for (size_t i = 0; i < n; i++) {
    res = (res * 10) + (str[i] - '0');
  }
  return res;
}

/**
 * ----------------------------------------------------------------------------
 */
static void parse_index(struct index *index, const char *tok,
                        const size_t len) {
  ssize_t curr = 0;
  ssize_t prev = 0;

  curr = find_delim('/', prev, tok, len);
  index->m_position_idx = strntoul(&tok[prev], curr - prev) - 1;
  prev = curr + 1;

  curr = find_delim('/', prev, tok, len);
  index->m_tex_coord_idx = strntoul(&tok[prev], curr - prev) - 1;
  prev = curr + 1;

  index->m_normal_idx = strntoul(&tok[prev], len - prev) - 1;
}

/**
 * ----------------------------------------------------------------------------
 */
static void parse_face(struct index *index, const char *line,
                       const size_t len) {
  ssize_t curr = 0;
  ssize_t prev = 0;

  curr = find_delim(' ', prev, line, len);
  parse_index(&index[0], &line[prev], curr - prev);
  prev = curr + 1;

  curr = find_delim(' ', prev, line, len);
  parse_index(&index[1], &line[prev], curr - prev);
  prev = curr + 1;

  curr = find_delim('\n', prev, line, len);
  parse_index(&index[2], &line[prev], curr - prev);
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
  struct index indices[f_count];

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
      parse_face(&indices[f_count], &lineptr[2], len);
      f_count += 3;
    }
    prev = curr + 1;
  }

  
  struct mesh mesh = {0};
  mesh.mp_vertices = malloc(f_count * sizeof(struct vertex));
  mesh.m_num_vertices = f_count;

  for (size_t i = 0; i < f_count; i++) {
    struct index index = indices[i];

    for (int j = 0; j < 3; j++) {
      mesh.mp_vertices[i].m_position[j] = positions[index.m_position_idx][j];
    }

    for (int j = 0; j < 2; j++) {
      mesh.mp_vertices[i].m_tex_coord[j] = tex_coords[index.m_tex_coord_idx][j];
    }

    for (int j = 0; j < 3; j++) {
      mesh.mp_vertices[i].m_normal[j] = normals[index.m_normal_idx][j];
    }
  }

  /*
#ifndef MESH_GTEST
  glGenBuffers(1, &mesh.m_vbo);
  glGenVertexArrays(1, &mesh.m_vao);
  glBindVertexArray(mesh.m_vao);

  glBindBuffer(GL_ARRAY_BUFFER, mesh.m_vbo);
  glBufferData(GL_ARRAY_BUFFER, mesh.m_num_vertices * sizeof(struct vertex),
               (void *)(mesh.mp_vertices), GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(offsetof(struct vertex, m_normal)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(offsetof(struct vertex, m_tex_coord)));
  glEnableVertexAttribArray(2);
#endif
*/

  mmapfile_unmap(&file);
  return mesh;
}

/**
 * ----------------------------------------------------------------------------
 */
void mesh_unload(struct mesh *mesh) {
  free(mesh->mp_vertices);
  // mesh->mp_vertices = NULL;
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
