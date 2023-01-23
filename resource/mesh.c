#include "mesh.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stb_image.h"

/**
 * ----------------------------------------------------------------------------
 */
struct texture load_texture(const char *obj_filepath,
                            const char *texture_filename, size_t len) {
  char *last_slash = strrchr(obj_filepath, '/');
  size_t dir_len = last_slash - obj_filepath + 1;
  size_t texture_filepath_len = dir_len + len;

  char texture_filepath[texture_filepath_len];
  texture_filepath[texture_filepath_len] = '\0';

  strncpy(texture_filepath, obj_filepath, dir_len);
  strncpy(&texture_filepath[dir_len], texture_filename, len);

  stbi_set_flip_vertically_on_load(true);
  struct texture tex = {0};
  tex.mp_data = stbi_load(texture_filepath, &tex.m_width, &tex.m_height,
                          &tex.m_num_channels, 0);
  return tex;
}

/**
 * ----------------------------------------------------------------------------
 */
struct mesh load_mesh(const char *filepath) {
  struct mesh mesh = {0};
  FILE *file = fopen(filepath, "r");
  if (file == NULL) {
    perror("fopen");
  }

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
        vertices[i].m_position[0] = strtof(line, &endptr);
        line = endptr;
        vertices[i].m_position[1] = strtof(line, &endptr);
        line = endptr;
        vertices[i].m_position[2] = strtof(line, &endptr);
        line = endptr;
        vertices[i].m_normal[0] = strtof(line, &endptr);
        line = endptr;
        vertices[i].m_normal[1] = strtof(line, &endptr);
        line = endptr;
        vertices[i].m_normal[2] = strtof(line, &endptr);
        line = endptr;
        vertices[i].m_tex_coord[0] = strtof(line, &endptr);
        line = endptr;
        vertices[i].m_tex_coord[1] = strtof(line, &endptr);
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
        if (strncmp(line, "(null)", 6) == 0) {
          break;
        }
        // TODO load_texture
        printf("%s", line);
      }
    }
  }

  for (uint32_t i = 0; i < mesh.m_num_vertices; i++) {
    printf("%d - %f %f %f %f %f %f %f %f\n", i, vertices[i].m_position[0],
           vertices[i].m_position[1], vertices[i].m_position[2],
           vertices[i].m_normal[0], vertices[i].m_normal[1],
           vertices[i].m_normal[2], vertices[i].m_tex_coord[0],
           vertices[i].m_tex_coord[1]);
  }

  free(vertices);
  free(indices);
  fclose(file);
  return mesh;
}

/**
 * ----------------------------------------------------------------------------
 */
void unload_mesh(struct mesh *mesh) {
  free(mesh->mp_vertices);
  // TODO this is temporary
  stbi_image_free(mesh->m_material.m_tex_normal.mp_data);
  stbi_image_free(mesh->m_material.m_tex_diffuse.mp_data);
  stbi_image_free(mesh->m_material.m_tex_specular.mp_data);
}
