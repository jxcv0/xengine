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

#define MAX_MESHES 128

/**
 * ----------------------------------------------------------------------------
 */
uint32_t load_texture(const char *filename) {
  size_t namelen = strlen(filename) - 1;  // '\n'
  size_t dirlen = strlen(TEXTURE_DIR);
  char filepath[namelen + dirlen];
  filepath[namelen + dirlen] = '\0';

  strncpy(filepath, TEXTURE_DIR, dirlen);
  strncpy(&filepath[dirlen], filename, namelen);

  printf("Loading texture from %s.\n", filepath);

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
struct mesh load_mesh(const char *filename) {
  size_t namelen = strlen(filename);
  size_t dirlen = strlen(MESH_DIR);
  char filepath[namelen + dirlen + 1];
  filepath[namelen + dirlen] = '\0';

  strncpy(filepath, MESH_DIR, dirlen);
  strncpy(&filepath[dirlen], filename, namelen);

  printf("Loading mesh from %s.\n", filepath);

  struct mesh mesh = {0};
  FILE *file = fopen(filepath, "r");
  if (file == NULL) {
    perror("fopen");
  }

  // TODO make this into one malloc
  size_t linesize = 128;
  char *line = malloc(linesize);
  struct vertex *vertices = NULL;
  uint32_t *indices = NULL;

  ssize_t nread;
  while ((nread = getline(&line, &linesize, file)) != -1) {
    if (strstr(line, "MESHES") != NULL) {
      if (atoi(&line[7]) > 1) {
        fprintf(stderr, "Multiple meshes not supported. Loading first mesh.\n");
      }
    } else if (strstr(line, "VERTICES") != NULL) {
      mesh.m_num_vertices = atoi(&line[9]);
      vertices = malloc(sizeof(struct vertex) * mesh.m_num_vertices);
      char *endptr;
      for (uint32_t i = 0; i < mesh.m_num_vertices; i++) {
        getline(&line, &linesize, file);
        char *lineptr = line;
        vertices[i].m_position[0] = strtof(lineptr, &endptr);
        lineptr = endptr;
        vertices[i].m_position[1] = strtof(lineptr, &endptr);
        lineptr = endptr;
        vertices[i].m_position[2] = strtof(lineptr, &endptr);
        lineptr = endptr;
        vertices[i].m_normal[0] = strtof(lineptr, &endptr);
        lineptr = endptr;
        vertices[i].m_normal[1] = strtof(lineptr, &endptr);
        lineptr = endptr;
        vertices[i].m_normal[2] = strtof(lineptr, &endptr);
        lineptr = endptr;
        vertices[i].m_tex_coord[0] = strtof(lineptr, &endptr);
        lineptr = endptr;
        vertices[i].m_tex_coord[1] = strtof(lineptr, &endptr);
      }
    } else if (strstr(line, "INDICES") != NULL) {
      mesh.m_num_indices = atoi(&line[8]);
      indices = malloc(sizeof(uint32_t) * mesh.m_num_indices);
      for (uint32_t i = 0; i < mesh.m_num_indices; i++) {
        getline(&line, &linesize, file);
        indices[i] = atoi(line);
      }
    } else if (strstr(line, "TEXTURES") != NULL) {
      // TODO fix this - should not be getting (null) here
      getline(&line, &linesize, file);
      if (strncmp(line, "(null)", 6) != 0) {
        mesh.m_tex_diff = load_texture(line);
      }

      getline(&line, &linesize, file);
      if (strncmp(line, "(null)", 6) != 0) {
        mesh.m_tex_spec = load_texture(line);
      }
      // TODO normal map
    }
  }

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
  free(line);
  free(indices);

  fclose(file);
  return mesh;
}

/**
 * ----------------------------------------------------------------------------
 */
void unload_mesh(struct mesh *mesh) {
  glDeleteBuffers(1, &mesh->m_vbo);
  glDeleteBuffers(1, &mesh->m_ebo);
  glDeleteVertexArrays(1, &mesh->m_vao);
}

struct asset_array alloc_asset_array(size_t asset_size, const uint32_t count) {
  struct asset_array arr = {0};
  return arr;
}
