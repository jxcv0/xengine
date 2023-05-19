#include "assets.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * ----------------------------------------------------------------------------
 */
static char *load_file(const char *filepath) {
  FILE *file = fopen(filepath, "rb");
  if (file == NULL) {
    perror("fopen");
    return NULL;
  }

  if (fseek(file, 0, SEEK_END) == -1) {
    fclose(file);
    perror("fseek");
    return NULL;
  }

  long filesize = ftell(file);
  if (filesize == -1) {
    fclose(file);
    perror("ftell");
    return NULL;
  }

  char *buff = malloc(filesize + 1);  // + 1 for '\0'
  rewind(file);

  fread(buff, filesize, 1, file);

  fclose(file);
  buff[filesize] = '\0';

  return buff;
}

/**
 * ----------------------------------------------------------------------------
 */
int load_obj(struct geometry *geom, const char *filepath) {
  char *file = load_file(filepath);

  if (file == NULL) {
    return -1;
  }

  size_t header[2];
  char *endptr = file;
  for (int i = 0; i < 2; i++) {
    header[i] = strtol(endptr, &endptr, 10);
  }

  size_t verts_size = sizeof(struct vertex) * header[0];
  size_t indices_size = sizeof(GLuint) * header[1];
  struct vertex *vertices = malloc(verts_size);
  GLuint *indices = malloc(indices_size);

  memcpy(vertices, endptr + 1, verts_size);
  memcpy(indices, endptr + 1 + verts_size, indices_size);

  geom->num_vertices = header[0];
  geom->num_indices = header[1];

  glGenBuffers(1, &geom->vbo);
  glGenBuffers(1, &geom->ebo);
  glGenVertexArrays(1, &geom->vao);
  glBindVertexArray(geom->vao);

  glBindBuffer(GL_ARRAY_BUFFER, geom->vbo);
  glBufferData(GL_ARRAY_BUFFER, geom->num_vertices * sizeof(struct vertex),
               vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, geom->num_indices * sizeof(uint32_t),
               indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(offsetof(struct vertex, position)));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(offsetof(struct vertex, tangent)));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(offsetof(struct vertex, bitangent)));

  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(offsetof(struct vertex, normal)));

  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(offsetof(struct vertex, tex_coord)));

  free(vertices);
  free(indices);
  free(file);

  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
int load_mtl(struct pbr_material *mat, const char *diffuse, const char *normal,
             const char *roughness, const char *metallic) {
  if (mat == NULL) {
    return -1;
  }

  const char *in[] = {diffuse, normal, roughness, metallic};
  char *arr[4];
  for (int i = 0; i < 4; i++) {
    char *file = load_file(in[i]);
    if (file == NULL) {
      for (int j = 0; j < i; j++) {
        free(arr[j]);
      }
      return -1;
    }
    arr[i] = file;
  }

  for (int i = 0; i < 4; i++) {
    size_t whn[3];
    char *endptr = arr[i];
    for (int i = 0; i < 3; i++) {
      whn[i] = strtol(endptr, &endptr, 10);
    }

    static const GLint format_table[] = {GL_RGBA, GL_RED, GL_RGBA, GL_RGB,
                                         GL_RGBA};

    glGenTextures(1, &mat->arr[i]);
    glBindTexture(GL_TEXTURE_2D, mat->arr[i]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, format_table[whn[2]], whn[0], whn[1], 0,
                 format_table[whn[2]], GL_UNSIGNED_BYTE, endptr + 1);
  }

  for (int i = 0; i < 4; i++) {
    free(arr[i]);
  }
  return 0;
}
