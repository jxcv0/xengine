#include "mesh.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mmapfile.h"
#include "stb_image.h"

/**
 * ----------------------------------------------------------------------------
 */
struct index {
  size_t m_position_idx;
  size_t m_tex_coord_idx;
  size_t m_normal_idx;
};

/**
 * ----------------------------------------------------------------------------
 */
static ssize_t find_delim(const char c, const size_t pos, const char *s,
                          const size_t len) {
  for (size_t i = pos; i < len; i++) {
    char ch = s[i];
    if (ch == c || ch == '\n') {
      return i;
    }
  }
  return -1;
}

/**
 * ----------------------------------------------------------------------------
 */
static void parse_vec3(float *v, const char *line) {
  const char *p = line;
  char *end;
  v[0] = strtof(p, &end);
  p = end;
  v[1] = strtof(p, &end);
  p = end;
  v[2] = strtof(p, &end);
}

/**
 * ----------------------------------------------------------------------------
 */
static void parse_vec2(float *v, const char *line) {
  const char *p = line;
  char *end;
  v[0] = strtof(p, &end);
  p = end;
  v[1] = strtof(p, &end);
}

/**
 * ----------------------------------------------------------------------------
 */
static size_t strntoul(const char *str, size_t n) {
  size_t res = 0;
  for (size_t i = 0; i < n; i++) {
    res = (res * 10) + (str[i] - '0');
  }
  return res;
}

/**
 * ----------------------------------------------------------------------------
 */
static void parse_index(struct index *index, const char *tok,
                        const size_t len) {
  ssize_t curr = 0;
  ssize_t prev = 0;

  curr = find_delim('/', prev, tok, len);
  index->m_position_idx = strntoul(&tok[prev], curr - prev) - 1;
  prev = curr + 1;

  curr = find_delim('/', prev, tok, len);
  index->m_tex_coord_idx = strntoul(&tok[prev], curr - prev) - 1;
  prev = curr + 1;

  index->m_normal_idx = strntoul(&tok[prev], len - prev) - 1;
}

/**
 * ----------------------------------------------------------------------------
 */
static void parse_face(struct index *index, const char *line,
                       const size_t len) {
  ssize_t curr = 0;
  ssize_t prev = 0;

  curr = find_delim(' ', prev, line, len);
  parse_index(&index[0], &line[prev], curr - prev);
  prev = curr + 1;

  curr = find_delim(' ', prev, line, len);
  parse_index(&index[1], &line[prev], curr - prev);
  prev = curr + 1;

  curr = find_delim('\n', prev, line, len);
  parse_index(&index[2], &line[prev], curr - prev);
}

/**
 * ----------------------------------------------------------------------------
 */
static ssize_t get_line(const struct mmapfile *file, const ssize_t pos) {
  for (size_t i = pos; i < file->m_size; i++) {
    char c = ((char *)file->mp_addr)[i];
    if (c == '\n') {
      return i;
    }
  }
  return -1;
}

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
struct material load_material(const char *filepath) {
  // get the .mtl file
  size_t n = strlen(filepath);
  char mtl_filepath[n];
  strncpy(mtl_filepath, filepath, n);
  char *ext = strrchr(filepath, '.');
  size_t idx = ext - filepath;
  mtl_filepath[idx + 1] = 'm';
  mtl_filepath[idx + 2] = 't';
  mtl_filepath[idx + 3] = 'l';
  mtl_filepath[idx + 4] = '\0';

  // parse file
  struct mmapfile file = mmapfile_map(mtl_filepath);
  assert(file.mp_addr != NULL);

  struct material material = {0};

  ssize_t curr = 0;
  ssize_t prev = 0;
  while ((curr = get_line(&file, prev)) != -1) {
    char *lineptr = &((char *)file.mp_addr)[prev];

    if (strncmp(lineptr, "Ns ", 3) == 0) {
      material.m_specular_exponent = atof(&lineptr[3]);

    } else if (strncmp(lineptr, "Ka ", 3) == 0) {
      parse_vec3(material.m_ambient_color, &lineptr[3]);

    } else if (strncmp(lineptr, "Kd ", 3) == 0) {
      parse_vec3(material.m_diffuse_color, &lineptr[3]);

    } else if (strncmp(lineptr, "Ks ", 3) == 0) {
      parse_vec3(material.m_specular_color, &lineptr[3]);

    } else if (strncmp(lineptr, "map_Kd ", 7) == 0) {
      size_t len = curr - prev;
      material.m_tex_diffuse = load_texture(filepath, &lineptr[7], len - 7);

    } else if (strncmp(lineptr, "map_Bump ", 9) == 0) {
      size_t len = curr - prev;
      material.m_tex_normal = load_texture(filepath, &lineptr[9], len - 9);

    } else if (strncmp(lineptr, "map_Ks ", 7) == 0) {
      size_t len = curr - prev;
      material.m_tex_specular = load_texture(filepath, &lineptr[7], len - 7);
    }

    prev = curr + 1;
  }

  mmapfile_unmap(&file);
  return material;
}

/**
 * ----------------------------------------------------------------------------
 */
static struct mesh load_mesh_from_wfobj(const char *filepath) {
  struct mmapfile file = mmapfile_map(filepath);
  assert(file.mp_addr != NULL);

  size_t v_count = 0;
  size_t vt_count = 0;
  size_t vn_count = 0;
  size_t f_count = 0;

  // count up how much memory to allocate.
  ssize_t curr = 0;
  ssize_t prev = 0;
  while ((curr = get_line(&file, prev)) != -1) {
    char *lineptr = &((char *)file.mp_addr)[prev];
    if (strncmp(lineptr, "v ", 2) == 0) {
      v_count++;
    } else if (strncmp(lineptr, "vt ", 3) == 0) {
      vt_count++;
    } else if (strncmp(lineptr, "vn ", 3) == 0) {
      vn_count++;
    } else if (strncmp(lineptr, "f ", 2) == 0) {
      f_count += 3;  // 3 faces per line
    }
    prev = curr + 1;
  }

  size_t positions_size = sizeof(vec3) * v_count;
  size_t tex_coords_size = sizeof(vec2) * vt_count;
  size_t normals_size = sizeof(vec3) * vn_count;
  size_t indices_size = sizeof(struct index) * f_count;
  size_t mem_size =
      positions_size + tex_coords_size + normals_size + indices_size;
  void *temp_mem = malloc(mem_size);

  vec3 *positions = (vec3 *)(temp_mem);
  vec2 *tex_coords = (vec2 *)(temp_mem + positions_size);
  vec3 *normals = (vec3 *)(temp_mem + positions_size + tex_coords_size);
  struct index *indices = (struct index *)(temp_mem + positions_size +
                                           tex_coords_size + normals_size);

  struct mesh mesh = {0};
  v_count = 0;
  vt_count = 0;
  vn_count = 0;
  f_count = 0;

  curr = 0;
  prev = 0;
  while ((curr = get_line(&file, prev)) != -1) {
    char *lineptr = &((char *)file.mp_addr)[prev];
    if (strncmp(lineptr, "v ", 2) == 0) {
      parse_vec3(positions[v_count++], &lineptr[2]);

    } else if (strncmp(lineptr, "vt ", 3) == 0) {
      parse_vec2(tex_coords[vt_count++], &lineptr[3]);

    } else if (strncmp(lineptr, "vn ", 3) == 0) {
      parse_vec3(normals[vn_count++], &lineptr[3]);

    } else if (strncmp(lineptr, "f ", 2) == 0) {
      size_t len = curr - prev;
      parse_face(&indices[f_count], &lineptr[2], len);
      f_count += 3;

    } else if (strncmp(lineptr, "mtllib ", 7) == 0) {
      mesh.m_material = load_material(filepath);
    }

    prev = curr + 1;
  }

  mesh.mp_vertices = malloc(f_count * sizeof(struct vertex));
  mesh.m_num_vertices = f_count;

  for (size_t i = 0; i < f_count; i++) {
    struct index index = indices[i];

    for (int j = 0; j < 3; j++) {
      mesh.mp_vertices[i].m_position[j] = positions[index.m_position_idx][j];
    }

    for (int j = 0; j < 2; j++) {
      mesh.mp_vertices[i].m_tex_coord[j] = tex_coords[index.m_tex_coord_idx][j];
    }

    for (int j = 0; j < 3; j++) {
      mesh.mp_vertices[i].m_normal[j] = normals[index.m_normal_idx][j];
    }
  }

  free(temp_mem);
  mmapfile_unmap(&file);
  return mesh;
}

struct mesh load_mesh(const char *filepath) {
  struct mesh mesh = {0};
  char *ext = strchr(filepath, '.');
  if (strcmp(ext, ".obj") == 0) {
    mesh = load_mesh_from_wfobj(filepath);
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
