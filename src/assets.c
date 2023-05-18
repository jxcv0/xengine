#include "assets.h"

#include <assert.h>
#include <ctype.h>
#include <libgen.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stb_image.h"

/**
 * ----------------------------------------------------------------------------
 */
static char *load_file(const char *filepath, size_t *size) {
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

  *size = fread(buff, filesize, 1, file);

  fclose(file);
  buff[filesize] = '\0';

  return buff;
}

/**
 * ----------------------------------------------------------------------------
 */
int load_obj(struct geometry *geom, const char *filepath) {
  printf("Loading geometry from \"%s\".\n", filepath);
  size_t filesize;
  char *file = load_file(filepath, &filesize);

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

/**
 * ----------------------------------------------------------------------------
 */
int load_pbr_material(struct pbr_material *mat, const char *material_name) {
  if (mat == NULL) {
    return -1;
  }

  char buff[64] = {0};
  size_t n = strlen(material_name);
  strncpy(buff, material_name, n);

  strncpy(&buff[n], "_diffuse.png", 13);
  mat->diffuse = load_texture(buff);

  strncpy(&buff[n], "_normal.png", 12);
  mat->normal = load_texture(buff);

  strncpy(&buff[n], "_roughness.png", 15);
  mat->roughness = load_texture(buff);

  strncpy(&buff[n], "_metallic.png", 14);
  mat->metallic = load_texture(buff);

  // strncpy(&buff[n], "_displacement.png", 18);
  // mat.displacement = load_texture(buff);

  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
static GLuint prv_load_texture(const char *filepath) {
  static GLint format_table[] = {GL_RGBA, GL_RED, GL_RGBA, GL_RGB, GL_RGBA};
  printf("Loading texture from \"%s\".\n", filepath);
  // stbi_set_flip_vertically_on_load(true);
  int w, h, n;
  unsigned char *data = stbi_load(filepath, &w, &h, &n, 0);
  assert(data != NULL);  // filename may not exist.

  uint32_t id;
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, format_table[n], w, h, 0, format_table[n],
               GL_UNSIGNED_BYTE, data);

  free(data);
  return id;
}

/**
 * ----------------------------------------------------------------------------
 */
GLuint load_texture(const char *filename) {
  size_t namelen = strlen(filename);
  size_t dirlen = strlen(TEXTURE_DIR);
  char filepath[namelen + dirlen];
  strncpy(filepath, TEXTURE_DIR, dirlen + 1);
  strncpy(&filepath[dirlen], filename, namelen + 1);
  return prv_load_texture(filepath);
}
