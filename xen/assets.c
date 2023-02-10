#include "assets.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glad.h"
#include "stb_image.h"
#include "utils.h"

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
                    const char *file, const uint32_t num_meshes) {
  for (uint32_t i = 0; i < num_meshes; i++) {
    char *pos = strstr(file, "VERTICES");
    vertex_counts[i] = (uint32_t)strtol(&pos[9], NULL, 10);
    assert((pos = strchr(pos, '\n') + 1) != NULL);  // +1 for '\n'
    size_t n = sizeof(struct vertex) * vertex_counts[i];
    vertices[i] = malloc(n);
    memcpy(vertices[i], pos, n);
    pos += n;
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

  const char *file = load_file_into_mem(filepath);
  assert(file != NULL);
  char *pos = (char*)file;

  // MESHES header
  uint32_t num_meshes = 0;
  if ((pos = strstr(pos, "MESHES")) != NULL) {
    num_meshes = atoi(&pos[7]);
    printf("Attempting to load %u meshes.\n", num_meshes);
  }

  struct vertex *vertices[num_meshes];
  uint32_t vertex_counts[num_meshes];

#pragma omp parallel
  {
#pragma omp single
    {
#pragma omp task
      { parse_vertices(vertices, &vertex_counts, file, num_meshes); }
    }
  }

  for (uint32_t i = 0; i < num_meshes; i++) {
    struct mesh mesh = {0};
    struct vertex *vertices = NULL;
    uint32_t *indices = NULL;
    char diffuse_name[64] = {0};
    char specular_name[64] = {0};

    // INDICES section
    if ((pos = strstr(pos, "INDICES")) != NULL) {
      mesh.m_num_indices = atoi(&pos[8]);
      assert((pos = strchr(pos, '\n') + 1) != NULL);  // +1 for '\n'
      size_t n = sizeof(uint32_t) * mesh.m_num_indices;
      indices = malloc(n);
      memcpy(indices, pos, n);
      pos += n;
    }

    if ((pos = strstr(pos, "DIFFUSE")) != NULL) {
      pos = strchr(pos, '\n') + 1;
      char *end = strchr(pos, '\n');
      size_t n = end - pos;
      strncpy(diffuse_name, pos, n);
      pos += n;
    }

    if ((pos = strstr(pos, "SPECULAR")) != NULL) {
      pos = strchr(pos, '\n') + 1;
      char *end = strchr(pos, '\n');
      size_t n = end - pos;
      strncpy(specular_name, pos, n);
      pos += n;
    }

    if (strncmp(diffuse_name, "(null)", 6) != 0) {
      mesh.m_tex_diff = load_texture(diffuse_name);
    }

    if (strncmp(specular_name, "(null)", 6) != 0) {
      mesh.m_tex_spec = load_texture(specular_name);
    }

    assert(vertices != NULL && indices != NULL);

    glGenBuffers(1, &mesh.m_vbo);
    glGenBuffers(1, &mesh.m_ebo);
    glGenVertexArrays(1, &mesh.m_vao);
    glBindVertexArray(mesh.m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.m_vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.m_num_vertices * sizeof(struct vertex),
                 (void *)(vertices), GL_STATIC_DRAW);

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
    free(vertices);
    free(indices);
    free(file);

    meshes[(*count)++] = mesh;
  }
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
