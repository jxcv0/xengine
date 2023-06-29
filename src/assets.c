#include "assets.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MESH_CACHE_SIZE 64
// static struct mesh mesh_cache[MESH_CACHE_SIZE];
// static size_t num_cached_meshes;

/**
 * ----------------------------------------------------------------------------
 */
char *load_file(const char *filepath) {
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

  if (fread(buff, 1, filesize, file) == 0) {
    free(buff);
    return NULL;
  }

  fclose(file);
  buff[filesize] = '\0';

  return buff;
}

/**
 * ----------------------------------------------------------------------------
 */
char *load_file_size(const char *filepath, size_t *len) {
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

  *len = fread(buff, 1, filesize, file);
  printf("%ld\n", *len);

  fclose(file);
  buff[filesize] = '\0';

  return buff;
}

/**
 * ----------------------------------------------------------------------------
 */
struct lz77tok *compress_lz77(const char *data, size_t len, size_t *ntoks) {
  struct lz77tok *toks = malloc(sizeof(struct lz77tok) * len);
  size_t n = 0;
  for (size_t i = 0; i < len;) {
    struct lz77tok tok = {.start = 0, .len = 0, .next = data[i]};
    const char *input = data + i;
    for (size_t j = 1; j <= i && j <= 64; j++) {
      const char *win = input - j;
      for (size_t k = 1; k <= 64; k++) {
        if (memcmp(input, win, k) == 0 && k > tok.len) {
          tok.start = j;
          tok.len = k;
          tok.next = input[k];
        }
      }
    }
    i += tok.len + 1;
    toks[n++] = tok;
  }
  *ntoks = n;
  return toks;
}

/**
 * ----------------------------------------------------------------------------
 */
char *decompress_lz77(struct lz77tok *toks, size_t ntoks, size_t n) {
  char *out = malloc(n);
  size_t idx = 0;
  for (size_t i = 0; i < ntoks; i++) {
    memcpy(out + idx, out + (idx - toks[i].start), toks[i].len);
    idx += toks[i].len;
    out[idx++] = toks[i].next;
  }

  return out;
}

/**
 * ----------------------------------------------------------------------------
 */
int load_mesh(struct mesh *mesh, const char *filepath) {
  FILE *file;
  if ((file = fopen(filepath, "rb")) == NULL) {
    return -1;
  }

  strcpy(mesh->asset_path, filepath);
  if (fread(&mesh->num_vertices, sizeof(size_t), 1, file) == 0) {
    fclose(file);
    return -1;
  }

  if (fread(&mesh->num_indices, sizeof(size_t), 1, file) == 0) {
    fclose(file);
    return -1;
  }

  size_t verts_size = sizeof(struct vertex) * mesh->num_vertices;
  size_t indices_size = sizeof(GLuint) * mesh->num_indices;

  void *mem = malloc(verts_size + indices_size);
  struct vertex *vertices = mem;
  GLuint *indices = (GLuint *)((uintptr_t)mem + verts_size);
  if (fread(mem, 1, verts_size + indices_size, file) == 0) {
    free(mem);
    fclose(file);
    return -1;
  }

  glGenBuffers(1, &mesh->vbo);
  glGenBuffers(1, &mesh->ebo);
  glGenVertexArrays(1, &mesh->vao);
  glBindVertexArray(mesh->vao);

  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
  glBufferData(GL_ARRAY_BUFFER, mesh->num_vertices * sizeof(struct vertex),
               vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->num_indices * sizeof(uint32_t),
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

  free(mem);
  fclose(file);

  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
int load_mtl(struct pbr_material *mat, const char *filepath) {
  FILE *file;
  if ((file = fopen(filepath, "rb")) == NULL) {
    return -1;
  }

  uint32_t sizes[4][3];
  if (fread(sizes, sizeof(uint32_t), 12, file) == 0) {
    fclose(file);
    return -1;
  }

  size_t imgsize = 0;
  for (int i = 0; i < 4; i++) {
    imgsize += sizes[i][0] * sizes[i][1] * sizes[i][2];
  }

  unsigned char *imgdata = malloc(imgsize);
  if (fread(imgdata, 1, imgsize, file) == 0) {
    fclose(file);
    free(imgdata);
    return -1;
  }

  size_t offset = 0;
  for (int i = 0; i < 4; i++) {
    static const GLint format_table[] = {GL_RGBA, GL_RED, GL_RGBA, GL_RGB,
                                         GL_RGBA};
    glGenTextures(1, &mat->arr[i]);
    glBindTexture(GL_TEXTURE_2D, mat->arr[i]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, format_table[sizes[i][2]], sizes[i][0],
                 sizes[i][1], 0, format_table[sizes[i][2]], GL_UNSIGNED_BYTE,
                 imgdata + offset);
    offset += sizes[i][0] * sizes[i][1] * sizes[i][2];
  }

  free(imgdata);
  fclose(file);

  return 0;
}

/**
 * ----------------------------------------------------------------------------
 */
int asset_type(const char *path) {
  char *ext = strrchr(path, '.');

  if (ext == NULL) {
    return -1;
  }

  const char *supported_asset_exts[] = {".mesh", ".mtl"};
  for (int i = 0; i < NUM_ASSET_TYPES; i++) {
    if (strcmp(supported_asset_exts[i], ext) == 0) {
      return i;
    }
  }

  return -1;
}
