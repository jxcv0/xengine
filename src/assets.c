#include "assets.h"

#include <assert.h>
#include <libgen.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glad.h"
#include "stb_image.h"

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
    perror("fseek");
    return NULL;
  }

  long filesize = ftell(file);
  if (filesize == -1) {
    perror("ftell");
    return NULL;
  }

  char *buff = malloc(filesize + 1);  // + 1 for '\0'
  rewind(file);

  size_t nread = fread(buff, filesize, 1, file);
  (void)nread;

  fclose(file);
  buff[filesize] = '\0';

  return buff;
}


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
int load_geometry(struct geometry *geom, const char *filepath) {
  if (geom == NULL) { return -1; }

  printf("Loading geometry from \"%s\".\n", filepath);
  char *file = load_file(filepath);

  printf("%s\n", file);
  return -1;
  // vertices
  char *pos = strstr(file, "VERTICES");
  if (pos == NULL) {
    fprintf(stderr, "VERTICES tag not found.\n");
    free(file);
    return -1;
  }

  geom->m_num_vertices = strtol(&pos[9], NULL, 10);
  if (pos == NULL) {
    free(file);
    return -1;
  }

  size_t n = sizeof(struct vertex) * geom->m_num_vertices;
  struct vertex *vertices = calloc(geom->m_num_vertices, sizeof(struct vertex));
  if (vertices == NULL) {
    perror("calloc");
    free(file);
    return -1;
  }

  memcpy(vertices, pos, n);
  pos += n;

  pos = strstr(file, "INDICES");
  if (pos == NULL) {
    fprintf(stderr, "INDICES tag not found.\n");
    free(file);
    return -1;
  }

  geom->m_num_indices = strtol(&pos[8], NULL, 10);
  if (pos == NULL) {
    free(file);
    return -1;
  }

  n = sizeof(uint32_t) * geom->m_num_indices;
  uint32_t *indices = calloc(geom->m_num_indices, sizeof(uint32_t));
  if (indices == NULL) {
    perror("calloc");
    free(vertices);
    free(file);
    return -1;
  }

  memcpy(indices, pos, n);

  glGenBuffers(1, &geom->m_vbo);
  glGenBuffers(1, &geom->m_ebo);
  glGenVertexArrays(1, &geom->m_vao);
  glBindVertexArray(geom->m_vao);

  glBindBuffer(GL_ARRAY_BUFFER, geom->m_vbo);
  glBufferData(GL_ARRAY_BUFFER, geom->m_num_vertices * sizeof(struct vertex),
               vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom->m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, geom->m_num_indices * sizeof(uint32_t),
               indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(offsetof(struct vertex, m_position)));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(offsetof(struct vertex, m_tangent)));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(offsetof(struct vertex, m_bitangent)));

  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(offsetof(struct vertex, m_normal)));

  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(offsetof(struct vertex, m_tex_coord)));

  free(vertices);
  free(indices);
  free(file);

  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
int load_pbr_material(struct pbr_material *mat, const char *material_name) {
  if (mat == NULL) { return -1; }

  char buff[64] = {0};
  size_t n = strlen(material_name);
  strncpy(buff, material_name, n);

  strncpy(&buff[n], "_diffuse.png", 13);
  mat->m_diffuse = load_texture(buff);

  strncpy(&buff[n], "_normal.png", 12);
  mat->m_normal = load_texture(buff);

  strncpy(&buff[n], "_roughness.png", 15);
  mat->m_roughness = load_texture(buff);

  strncpy(&buff[n], "_metallic.png", 14);
  mat->m_metallic = load_texture(buff);

  // strncpy(&buff[n], "_displacement.png", 18);
  // mat.m_displacement = load_texture(buff);

  return 0;
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
