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
#include "mmapfile.h"
#include "stb_image.h"

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


GLint format_table[] = {GL_RGBA, GL_RED, GL_RGBA, GL_RGB, GL_RGBA};

/**
 * ----------------------------------------------------------------------------
 */
static uint32_t do_texture_loading(const char *filepath) {
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

  glTexImage2D(GL_TEXTURE_2D, 0, format_table[n], w, h, 0, format_table[n], GL_UNSIGNED_BYTE,
               data);

  free(data);
  return id;
}

/**
 * ----------------------------------------------------------------------------
 */
int32_t provision_geometry(struct geometry *arr, size_t *table,
                           const size_t size, size_t *count) {
  return -1;
}

/**
 * ----------------------------------------------------------------------------
 */
int32_t release_geometry(struct geometry *arr, size_t *table, const size_t size,
                         size_t *count, int32_t handle) {
  size_t last;
  // get the handle of the last geometry
  for (size_t i = 0; i < *count; i++) {
    if (table[i] == *count) {
      last = i;
    }
  }

  arr[table[handle]] = arr[table[last]];
  table[last] = table[handle];

  (*count)--;
  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
struct geometry load_geometry(const char *filepath) {
  printf("Loading geometry from \"%s\".\n", filepath);
  size_t file_size = 0;
  const char *file = map_file(&file_size, filepath);

  struct geometry geom = {0};

  // vertices
  const char *pos = findstr(file, "VERTICES", file_size);
  assert(pos != NULL);
  geom.m_num_vertices = (uint32_t)strtol(&pos[9], NULL, 10);
  assert((pos = strchr(pos, '\n') + 1) != NULL);
  size_t n = sizeof(struct vertex) * geom.m_num_vertices;
  struct vertex *vertices = calloc(geom.m_num_vertices, sizeof(struct vertex));
  memcpy(vertices, pos, n);
  pos += n;

  // indices
  pos = findstr(file, "INDICES", file_size);
  assert(pos != NULL);
  geom.m_num_indices = (uint32_t)strtol(&pos[8], NULL, 10);
  assert((pos = strchr(pos, '\n') + 1) != NULL);
  n = sizeof(uint32_t) * geom.m_num_indices;
  uint32_t *indices = calloc(geom.m_num_indices, sizeof(uint32_t));
  memcpy(indices, pos, n);

  glGenBuffers(1, &geom.m_vbo);
  glGenBuffers(1, &geom.m_ebo);
  glGenVertexArrays(1, &geom.m_vao);
  glBindVertexArray(geom.m_vao);

  glBindBuffer(GL_ARRAY_BUFFER, geom.m_vbo);
  glBufferData(GL_ARRAY_BUFFER, geom.m_num_vertices * sizeof(struct vertex),
               vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom.m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, geom.m_num_indices * sizeof(uint32_t),
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

  // TODO defer this.
  unmap_file((void *)file, file_size);
  free(vertices);
  free(indices);

  return geom;
}

/**
 * ----------------------------------------------------------------------------
 */
struct pbr_material load_pbr_material(const char *material_name) {
  char buff[64] = {0};
  struct pbr_material mat = {0};
  size_t n = strlen(material_name);
  strncpy(buff, material_name, n);

  strncpy(&buff[n], "_diffuse.png", 13);
  mat.m_diffuse = load_texture(buff);

  strncpy(&buff[n], "_normal.png", 12);
  mat.m_normal = load_texture(buff);

  strncpy(&buff[n], "_roughness.png", 15);
  mat.m_roughness = load_texture(buff);

  strncpy(&buff[n], "_metallic.png", 14);
  mat.m_metallic = load_texture(buff);

  // strncpy(&buff[n], "_displacement.png", 18);
  // mat.m_displacement = load_texture(buff);

  return mat;
}

/**
 * ----------------------------------------------------------------------------
 */
uint32_t load_texture(const char *filename) {
  size_t namelen = strlen(filename);
  size_t dirlen = strlen(TEXTURE_DIR);
  char filepath[namelen + dirlen];
  strncpy(filepath, TEXTURE_DIR, dirlen + 1);
  strncpy(&filepath[dirlen], filename, namelen + 1);
  return do_texture_loading(filepath);
}

/**
 * ----------------------------------------------------------------------------
 */
void load_model(const char *filepath, alloc_geometries alloc_geom) {
  printf("Loading model from \"%s\".\n", filepath);

  FILE *model_file = fopen(filepath, "r");
  if (model_file == NULL) {
    perror("fopen");
  }

  // get the directory
  char *dir_end = strrchr(filepath, '/');
  size_t dir_len = (dir_end - filepath) + 1;  // + 1 for '/'

  char buffer[256];
  strncpy(buffer, filepath, dir_len);

  size_t lineptr_len = 0;
  char *lineptr;
  ssize_t nread = -1;
  while ((nread = getline(&lineptr, &lineptr_len, model_file)) != -1) {
    // geometry
    char *start = lineptr;
    char *end = strchr(start, ' ');
    size_t len = end - start;
    strncpy(&buffer[dir_len], start, len);
    buffer[len + dir_len] = '\0';
    struct geometry *geom = alloc_geom(1);
    *geom = load_geometry(buffer);

    // diffuse
    start = end + 1;
    end = strchr(start, ' ');
    len = end - start;
    strncpy(&buffer[dir_len], start, len);
    buffer[len + dir_len] = '\0';
    geom->m_material.m_diffuse = do_texture_loading(buffer);

    // roughness
    start = end + 1;
    end = strchr(start, ' ');
    len = end - start;
    strncpy(&buffer[dir_len], start, len);
    buffer[len + dir_len] = '\0';
    geom->m_material.m_roughness = do_texture_loading(buffer);

    // normal
    start = end + 1;
    end = strchr(start, ' ');
    len = end - start;
    strncpy(&buffer[dir_len], start, len);
    buffer[len + dir_len] = '\0';
    geom->m_material.m_normal = do_texture_loading(buffer);

    // metallic
    start = end + 1;
    end = strchr(start, '\n');
    len = end - start;
    strncpy(&buffer[dir_len], start, len);
    buffer[len + dir_len] = '\0';
    geom->m_material.m_metallic = do_texture_loading(buffer);
  }
  free(lineptr);
  fclose(model_file);
}
