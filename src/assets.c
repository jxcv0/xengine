#include "assets.h"

#include <assert.h>
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
static void count_vline(char c, size_t *nv, size_t *nvn, size_t *nvt) {
  switch (c) {
    case ' ':
      ++(*nv);
      break;
    case 'n':
      ++(*nvn);
      break;
    case 't':
      ++(*nvt);
      break;
  }
}

/**
 * ----------------------------------------------------------------------------
 */
static void parse_v3(char *line, vec3 *v) {
  char *endptr = line;
  for (size_t i = 0; i < 3; i++) {
    (*v)[i] = strtof(endptr, &endptr);
    printf("%f ", (*v)[i]);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
static void parse_v2(char *line, vec2 *v) {
  char *endptr = line;
  for (size_t i = 0; i < 2; i++) {
    (*v)[i] = strtof(endptr, &endptr);
    printf("%f ", (*v)[i]);
  }
}

/**
 * ----------------------------------------------------------------------------
 */
static char *parse_vlines(char *vstart, size_t nposn, size_t nnorm, size_t ntex,
                          vec3 *posn, vec3 *norm, vec2 *tex) {
  for (size_t i = 0; i < nposn; i++) {
    parse_v3(&vstart[2], &posn[i]);
    vstart = strchr(vstart, '\n') + 1;  // might not need this
  }

  for (size_t i = 0; i < nnorm; i++) {
    parse_v3(&vstart[3], &norm[i]);
    vstart = strchr(vstart, '\n') + 1;
  }

  for (size_t i = 0; i < ntex; i++) {
    parse_v2(&vstart[3], &tex[i]);
    vstart = strchr(vstart, '\n') + 1;
  }

  return vstart;
}

/**
 * ----------------------------------------------------------------------------
 */
static char *parse_flines(char *fstart, size_t nf, vec3 *posn, vec3 *norms,
                          vec2 *tex, struct vertex *vertices) {
  size_t nverts = 0;
  for (size_t i = 0; i < nf; i++) {
    fstart = strpbrk(fstart, " \n") + 1;
    size_t indices[3];
    printf("%ld\n", i);
    if (*fstart == ' ') {
      char *endptr = fstart;
      for (int j = 0; j < 3; j++) {
        indices[j] = strtol(endptr, &endptr, 10) - 1;
      }
    } else {
      // calc tangent and bitangent for last 3 verts
      ++fstart;
      continue;
    }

    for (int i = 0; i < 3; i++) {
      vertices[nverts].position[i] = posn[indices[0]][i];
      vertices[nverts].normal[i]  = norms[indices[2]][i];
    }
    
    vertices[nverts].tex_coord[0] = tex[indices[1]][0];
    vertices[nverts].tex_coord[1]  = tex[indices[1]][1];
    ++nverts;
  }
  return fstart;
}

/**
 * ----------------------------------------------------------------------------
 */
static void handle_usemtl(char *line, struct pbr_material *mat) {}

/**
 * ----------------------------------------------------------------------------
 */
int load_obj_file(struct geometry *geom, struct pbr_material *mat,
                  const char *filepath) {
  char *file = load_file(filepath);
  if (file == NULL) {
    return -1;
  }

  size_t nv = 0;
  size_t nvn = 0;
  size_t nvt = 0;
  size_t nf = 0;

  char *ptr = file;
  do {
    ++ptr;
    switch (ptr[0]) {
      case 'v':
        count_vline(ptr[1], &nv, &nvn, &nvt);
        break;
      case 'f':
        ++nf;
        break;
      default:
        break;
    }
  } while ((ptr = strchr(ptr, '\n')) != NULL);

  // TODO this can be one malloc
  vec3 *posn = malloc(sizeof(vec3) * nv);
  vec3 *norms = malloc(sizeof(vec3) * nvn);
  vec2 *tex = malloc(sizeof(vec2) * nvt);
  struct vertex *vertices = malloc(sizeof(struct vertex) * nf * 3);  // triangulated

  ptr = file;
  do {
    ++ptr;
    switch (ptr[0]) {
      case 'v':
        ptr = parse_vlines(ptr, nv, nvn, nvt, posn, norms, tex);
        break;
      case 'f':
        ptr = parse_flines(ptr, nf, posn, norms, tex, vertices);
        break;
      case 'u':
        // multiple materials not supported.
        if (mat != NULL) {
          handle_usemtl(ptr, mat);
        }
        break;
      default:
        break;
    }
  } while ((ptr = strchr(ptr, '\n')) != NULL);

  free(file);
  free(posn);
  free(norms);
  free(tex);
  free(vertices);

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
int load_geometry(struct geometry *geom, const char *filepath) {
  if (geom == NULL) {
    return -1;
  }

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

  geom->num_vertices = strtol(&pos[9], NULL, 10);
  if (pos == NULL) {
    free(file);
    return -1;
  }

  size_t n = sizeof(struct vertex) * geom->num_vertices;
  struct vertex *vertices = calloc(geom->num_vertices, sizeof(struct vertex));
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

  geom->num_indices = strtol(&pos[8], NULL, 10);
  if (pos == NULL) {
    free(file);
    return -1;
  }

  n = sizeof(uint32_t) * geom->num_indices;
  uint32_t *indices = calloc(geom->num_indices, sizeof(uint32_t));
  if (indices == NULL) {
    perror("calloc");
    free(vertices);
    free(file);
    return -1;
  }

  memcpy(indices, pos, n);

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
GLuint load_texture(const char *filename) {
  size_t namelen = strlen(filename);
  size_t dirlen = strlen(TEXTURE_DIR);
  char filepath[namelen + dirlen];
  strncpy(filepath, TEXTURE_DIR, dirlen + 1);
  strncpy(&filepath[dirlen], filename, namelen + 1);
  return prv_load_texture(filepath);
}
