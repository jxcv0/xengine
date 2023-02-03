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

#define MAX_MESHES 128

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
void load_model(uint32_t entity_id, struct mesh *mesh_arr, uint32_t *count,
                const char *filename) {
  size_t namelen = strlen(filename);
  size_t dirlen = strlen(MESH_DIR);
  char filepath[namelen + dirlen + 1];
  filepath[namelen + dirlen] = '\0';

  strncpy(filepath, MESH_DIR, dirlen);
  strncpy(&filepath[dirlen], filename, namelen);

  printf("Loading mesh from \"%s\".\n", filepath);

  char *file = load_file_into_mem(filepath);
  assert(file != NULL);
  char *pos = file;

  uint32_t last_num_vertices = 0;
  struct vertex *vertices = NULL;
  uint32_t last_num_indices = 0;
  uint32_t *indices = NULL;

  // MESHES header
  uint32_t num_meshes = 0;
  if ((pos = strstr(pos, "MESHES")) != NULL) {
    num_meshes = atoi(&pos[7]);
    printf("Loading %u meshes.\n", num_meshes);
  } else {
    fprintf(stderr, "Unable to find MESHES start tag\n");
  }

  for (uint32_t i = 0; i < num_meshes; i++) {
    struct mesh mesh = {0};
    mesh.m_entity_id = entity_id;

    // VERTICES section
    if ((pos = strstr(pos, "VERTICES")) != NULL) {
      mesh.m_num_vertices += atoi(&pos[9]);
      assert((pos = strchr(pos, '\n') + 1) != NULL);  // +1 for '\n'
      size_t n = sizeof(struct vertex) * mesh.m_num_vertices;
      if (last_num_vertices < mesh.m_num_vertices) {
        vertices = realloc(vertices, n);
      }
      memcpy(vertices, pos, n);
      pos += n + 1;
    }

    // INDICES section
    if ((pos = strstr(pos, "INDICES")) != NULL) {
      mesh.m_num_indices += atoi(&pos[8]);
      assert((pos = strchr(pos, '\n') + 1) != NULL);  // +1 for '\n'
      size_t n = sizeof(uint32_t) * mesh.m_num_indices;
      if (last_num_indices < mesh.m_num_indices) {
        indices = realloc(indices, n);
      }
      memcpy(indices, pos, n);
      pos += n + 1;
    }

    // TEXTURES section
    // If the TEXTURES tag is found then we assume there are 3 lines to parse.
    // while renderer does not do normal maps we only load 2 textures.
    if ((pos = strstr(pos, "TEXTURES")) != NULL) {
      char *end = pos;

      assert((pos = strchr(pos, '\n') + 1) != NULL);  // +1 for '\n'
      assert((end = strchr(pos, '\n')) != NULL);
      size_t n = end - pos;
      char diffname[n];
      diffname[n] = 0;
      strncpy(diffname, pos, n);
      mesh.m_tex_diff = load_texture(diffname);

      assert((pos = strchr(pos, '\n') + 1) != NULL);  // +1 for '\n'
      assert((end = strchr(pos, '\n')) != NULL);
      n = end - pos;
      char specname[n];
      specname[n] = 0;
      strncpy(specname, pos, n);
      mesh.m_tex_spec = load_texture(specname);
    }

    // sanity check
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

    mesh_arr[(*count)++] = mesh;
    last_num_vertices = mesh.m_num_vertices;
    last_num_indices = mesh.m_num_indices;
  }

  free(vertices);
  free(indices);
  free(file);
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
