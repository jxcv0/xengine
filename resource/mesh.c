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

struct mesh load_mesh(const char *filepath) {
  struct mesh mesh = {0};
  FILE *file = fopen(filepath, "r");
  if (file == NULL) {
    perror("fopen");
  }

  size_t linesize = 128;
  char *line = malloc(linesize);

  ssize_t nread;
  while ((nread = getline(&line, &linesize, file)) != -1) {
    printf("%ld\n", nread);
  }
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
