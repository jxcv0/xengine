#include "assets.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glad.h"
#include "mmapfile.h"
#include "stb_image.h"

/**
 * ----------------------------------------------------------------------------
 */
bool xenstrncmp(const char *s1, const char *s2, size_t len) {
  for (size_t i = 0; i < len; i++) {
    if (s1[i] != s2[i]) {
      return false;
    }
  }
  return true;
}

/**
 * ----------------------------------------------------------------------------
 */
char *findstr(const char *haystack, const char *needle, const size_t len) {
  size_t needle_len = strlen(needle);
  for (size_t i = 0; i < len; i++) {
    const char *p = &haystack[i];
    if (xenstrncmp(p, needle, needle_len)) {
      return (char *)p;
    }
  }
  return NULL;
}

/**
 * ----------------------------------------------------------------------------
 */
uint32_t load_texture(const char *filename) {
  size_t namelen = strlen(filename);
  size_t dirlen = strlen(TEXTURE_DIR);
  char filepath[namelen + dirlen];
  filepath[namelen + dirlen] = '\0';

  strncpy(filepath, TEXTURE_DIR, dirlen);
  strncpy(&filepath[dirlen], filename, namelen);

  printf("Loading texture from \"%s\".\n", filepath);

  stbi_set_flip_vertically_on_load(true);
  int w, h, n;
  unsigned char *data = stbi_load(filepath, &w, &h, &n, 0);
  assert(data != NULL);

  uint32_t id;
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  int format = GL_RGB;
  switch (n) {
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

  glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE,
               data);

  free(data);
  return id;
}

/**
 * ----------------------------------------------------------------------------
 */
void parse_vertices(struct vertex **vertices, uint32_t *vertex_counts,
                    const char *file, const uint32_t num_meshes,
                    size_t file_size) {
  for (uint32_t i = 0; i < num_meshes; i++) {
    const char *pos = findstr(file, "VERTICES", file_size);
    assert(pos != NULL);
    vertex_counts[i] = (uint32_t)strtol(&pos[9], NULL, 10);
    assert((pos = strchr(pos, '\n') + 1) != NULL);  // +1 for '\n'
    const size_t n = sizeof(struct vertex) * vertex_counts[i];
    vertices[i] = malloc(n);
    memcpy(vertices[i], pos, n);
    pos += n;
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void parse_indices(uint32_t **indices, uint32_t *indice_counts,
                   const char *file, const uint32_t num_meshes,
                   size_t file_size) {
  for (uint32_t i = 0; i < num_meshes; i++) {
    const char *pos = findstr(file, "INDICES", file_size);
    assert(pos != NULL);
    indice_counts[i] = (uint32_t)strtol(&pos[8], NULL, 10);
    assert((pos = strchr(pos, '\n') + 1) != NULL);  // +1 for '\n'
    const size_t n = sizeof(uint32_t) * indice_counts[i];
    indices[i] = malloc(n);
    memcpy(indices[i], pos, n);
    pos += n;
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void parse_texture(char **texture_names, const char *texture_name,
                   const char *file, const uint32_t num_meshes,
                   size_t file_size) {
  for (uint32_t i = 0; i < num_meshes; i++) {
    const char *pos = findstr(file, texture_name, file_size);
    assert(pos != NULL);
    if ((pos = findstr(pos, texture_name, file_size)) != NULL) {
      pos = strchr(pos, '\n') + 1;
      char *end = strchr(pos, '\n');
      size_t n = end - pos + 1;
      memcpy(&texture_names[i][0], pos, n);
      texture_names[n] = '\0';
      pos += n;
    }
  }
}

/**
 * ----------------------------------------------------------------------------
 */
void load_mesh(struct mesh *meshes, uint32_t *count, const char *filename) {
  size_t namelen = strlen(filename);
  size_t dirlen = strlen(MESH_DIR);
  char filepath[namelen + dirlen + 1];
  filepath[namelen + dirlen] = '\0';
  strncpy(filepath, MESH_DIR, dirlen);
  strncpy(&filepath[dirlen], filename, namelen);

  size_t file_size = 0;
  const char *file = map_file(&file_size, filepath);

  // MESHES header
  uint32_t num_meshes = 0;
  const char *p = findstr(file, "MESHES", file_size);
  assert(p != NULL);
  num_meshes = strtol(&p[7], NULL, 10);
  assert(num_meshes != 0);
  printf("Attempting to load %u meshes.\n", num_meshes);

  // pointer arrays
  struct vertex **vertices = calloc(num_meshes, sizeof(struct vertex));
  uint32_t *vertex_counts = calloc(num_meshes, sizeof(uint32_t));
  uint32_t **indices = calloc(num_meshes, sizeof(uint32_t));
  uint32_t *indice_counts = calloc(num_meshes, sizeof(uint32_t));
  char **diffuse_textures = calloc(num_meshes, TEXTURE_NAME_LEN);
  char **specular_textures = calloc(num_meshes, TEXTURE_NAME_LEN);

  /*
#pragma omp parallel default(shared) firstprivate(file)
  {
#pragma omp single
    {
      // #pragma omp task
      { parse_vertices(vertices, vertex_counts, file, num_meshes); }
      // #pragma omp task
      { parse_indices(indices, indice_counts, file, num_meshes); }
      // #pragma omp task
      { parse_texture(diffuse_textures, "DIFFUSE", file, num_meshes); }
      // #pragma omp task
      { parse_texture(specular_textures, "SPECULAR", file, num_meshes); }
    }
  }
  */

  parse_vertices(vertices, vertex_counts, file, num_meshes, file_size);
  parse_indices(indices, indice_counts, file, num_meshes, file_size);


  parse_texture(diffuse_textures, "DIFFUSE", file, num_meshes, file_size);
  parse_texture(specular_textures, "SPECULAR", file, num_meshes, file_size);

  for (uint32_t i = 0; i < num_meshes; i++) {
    struct mesh mesh = {0};
    if (strncmp(diffuse_textures[i], "(null)", 6) != 0) {
      mesh.m_tex_diff = load_texture(diffuse_textures[i]);
    }

    if (strncmp(specular_textures[i], "(null)", 6) != 0) {
      mesh.m_tex_spec = load_texture(specular_textures[i]);
    }

    glGenBuffers(1, &mesh.m_vbo);
    glGenBuffers(1, &mesh.m_ebo);
    glGenVertexArrays(1, &mesh.m_vao);
    glBindVertexArray(mesh.m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_counts[i] * sizeof(struct vertex),
                 (void *)(vertices[i]), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * mesh.m_num_indices,
                 indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                          (void *)(offsetof(struct vertex, m_position)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                          (void *)(offsetof(struct vertex, m_normal)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                          (void *)(offsetof(struct vertex, m_tex_coord)));

    meshes[(*count)++] = mesh;
  }
  unmap_file((char *)file, file_size);
}

/**
 * ----------------------------------------------------------------------------
 */
void unload_mesh(struct mesh *mesh) {
  glDeleteBuffers(1, &mesh->m_vbo);
  glDeleteBuffers(1, &mesh->m_ebo);
  glDeleteVertexArrays(1, &mesh->m_vao);
  glDeleteTextures(1, &mesh->m_tex_spec);
  glDeleteTextures(1, &mesh->m_tex_diff);
}
