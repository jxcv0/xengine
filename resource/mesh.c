#include "mesh.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glad.h"
#include "stb_image.h"

#define MESH_DIR "assets/meshes/"
#define TEXTURE_DIR "assets/textures/"

/**
 * ----------------------------------------------------------------------------
 */
struct texture load_texture(const char *filename) {
  size_t namelen = strlen(filename);
  size_t dirlen = strlen(TEXTURE_DIR);
  char filepath[namelen + dirlen + 1];
  filepath[namelen + dirlen] = '\0';

  strncpy(filepath, MESH_DIR, dirlen);
  strncpy(&filepath[dirlen], filename, namelen);

  stbi_set_flip_vertically_on_load(true);
  struct texture tex = {0};
  tex.mp_data =
      stbi_load(filepath, &tex.m_width, &tex.m_height, &tex.m_num_channels, 0);
  return tex;
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
  struct texture textures[3] = {0};

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
      for (int i = 0; i < 3; i++) {
        getline(&line, &linesize, file);
        if (strncmp(line, "(null)", 6) != 0) {
          textures[i] = load_texture(line);
        }
      }
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
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(offsetof(struct vertex, m_position)));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(offsetof(struct vertex, m_normal)));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(offsetof(struct vertex, m_tex_coord)));

  // TODO what about textureless meshes
  for (int i = 0; i < 3; i++) {
    glGenTextures(1, &textures[i].m_texture_id);
    glBindTexture(GL_TEXTURE_2D, textures[i].m_texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    int format;
    switch (textures[i].m_num_channels) {
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

    glTexImage2D(GL_TEXTURE_2D, 0, format, textures[i].m_width,
                 textures[i].m_height, 0, format, GL_UNSIGNED_BYTE,
                 textures[i].mp_data);

    free(textures[i].mp_data);
  }

  mesh.m_tex_diff = textures[0].m_texture_id;
  mesh.m_tex_spec = textures[1].m_texture_id;
  // TODO normal map

  free(vertices);
  free(line);
  free(indices);

  fclose(file);
  return mesh;
}

void unload_mesh(struct mesh *mesh) {
  (void)mesh;
  // TODO free buffers
}
