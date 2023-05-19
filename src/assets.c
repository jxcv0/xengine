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
  printf("Loading geometry from \"%s\".\n", filepath);
  char *file = load_file(filepath);

  if (file == NULL) {
    return -1;
  }

  if (strncmp(file, "VERTICES", 8) != 0) {
    fprintf(stderr, "VERTICES tag not found.\n");
    return -1;
  }

  geom->num_vertices = strtol(&file[9], NULL, 10);

  size_t verts_size = sizeof(struct vertex) * geom->num_vertices;
  struct vertex *vertices = malloc(verts_size);
  if (vertices == NULL) {
    perror("malloc");
    free(file);
    return -1;
  }

  char *verts_start = strchr(file, '\n') + 1;
  memcpy(vertices, verts_start, verts_size);

  char *indices_tag = verts_start + verts_size + 1;
  if (strncmp(indices_tag, "INDICES", 7) != 0) {
    fprintf(stderr, "INDICES tag not found.\n");
    return -1;
  }

  geom->num_indices = strtol(&indices_tag[8], NULL, 10);

  size_t indices_size = sizeof(GLuint) * geom->num_indices;
  GLuint *indices = malloc(indices_size);
  if (indices == NULL) {
    perror("malloc");
    free(vertices);
    free(file);
    return -1;
  }

  char *indices_start = strchr(indices_tag, '\n') + 1;
  memcpy(indices, indices_start, indices_size);

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

int load_mtl(struct pbr_material *mat, const char *diffuse, const char *normal,
             const char *roughness, const char *metallic) {
  if (mat == NULL) {
    return -1;
  }

  char *arr[] = {load_file(diffuse), load_file(normal), load_file(roughness),
                 load_file(metallic)};

  for (int i = 0; i < 4; i++) {
    assert(arr[i] != NULL);

    size_t whn[3];
    char *endptr = arr[i];
    for (int i = 0; i < 3; i++) {
      whn[i] = strtol(endptr, &endptr, 10);
    }

    static GLint format_table[] = {GL_RGBA, GL_RED, GL_RGBA, GL_RGB, GL_RGBA};

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
