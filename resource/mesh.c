#include "mesh.h"

#include <glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "mmapfile.h"

struct index {
  unsigned int m_position_idx;
  unsigned int m_tex_coord_idx;
  unsigned int m_normal_idx;
};

/**
 * ----------------------------------------------------------------------------
Vec3 parse_vec3(const std::string_view &sv) {
  const char *p = sv.data();
  char *end;
  float result[3];
  int i = 0;
  for (float f = std::strtof(p, &end); p != end; f = strtof(p, &end)) {
    result[i++] = f;
    p = end;
  }
  return Vec3(result[0], result[1], result[2]);
}
 */

/**
 * ----------------------------------------------------------------------------
Vec2 parse_vec2(const std::string_view &sv) {
  const char *p = sv.data();
  char *end;
  float result[2];
  int i = 0;
  for (float f = std::strtof(p, &end); p != end; f = strtof(p, &end)) {
    result[i++] = f;
    p = end;
  }
  return Vec2(result[0], result[1]);
}
 */

/**
 * ----------------------------------------------------------------------------
Mesh::Index parse_index(const std::string_view &sv) {
  using size_type = std::string_view::size_type;
  size_type curr = 0;
  size_type prev = 0;

  Mesh::Index index;
  curr = sv.find('/', prev);
  auto len = curr - prev;
  auto pos_index = sv.substr(prev, len);
  index.m_position_idx = std::atoi(pos_index.data()) - 1;
  prev += len + 1;

  curr = sv.find('/', prev);
  len = curr - prev;
  auto tex_coords_index = sv.substr(prev, len);
  index.m_tex_coord_idx = std::atoi(tex_coords_index.data()) - 1;
  prev += len + 1;

  curr = sv.find('/', sv.size() - prev);
  len = curr - prev;
  auto normal_index = sv.substr(prev, len);
  index.m_normal_idx = std::atoi(normal_index.data()) - 1;

  return index;
}
 */

ssize_t get_line(const struct mmapfile *file, const ssize_t pos) {
  for (size_t i = 0; i < file->m_size - pos; i++) {
    char c = ((char *)file->mp_addr)[pos];
    if (c == '\n' || c == '\r') {
      return i;
    }
  }
  return -1;
}

/**
 * ----------------------------------------------------------------------------
 */
struct mesh mesh_load(const char *filepath) {
  const struct mmapfile file = mmapfile_map(filepath);
  struct mesh mesh = {0};

  size_t num_lines = 0;
  for (size_t i = 0; i < file.m_size; i++) {
    char c = ((char *)file.mp_addr)[i];
    if (c == '\n') {
      num_lines++;
    }
  }

  size_t eols[num_lines + 1];
  num_lines = 0;
  eols[num_lines++] = 0;
  for (size_t i = 0; i < file.m_size; i++) {
    char c = ((char *)file.mp_addr)[i];
    if (c == '\n') {
      eols[num_lines++] = i;
    }
  }

  for (size_t i = 0; i < num_lines; i++) {
    printf("%ld ", eols[i]);
  }

  /*
  // count up how much memory to allocate.
  unsigned int num_positions = 0;
  unsigned int num_tex_coords = 0;
  unsigned int num_normals = 0;

  size_t num_vertices = 0;

  // count up how much memory is requred before copying

  float positions[num_positions];
  float normals[num_normals];
  float tex_coords[num_tex_coords];
  struct index indices[num_vertices];

  num_positions = 0;
  num_tex_coords = 0;
  num_normals = 0;
  mesh.m_num_vertices = 0;

  curr = 0;
  prev = 0;
  while ((curr = view.find('\n', prev)) != std::string_view::npos) {
    auto len = curr - prev + 1;
    auto line = view.substr(prev, len);
    prev += len;

    if ("v " == line.substr(0, 2)) {
      positions[num_positions++] = parse_vec3(line.substr(2));
    } else if ("vt " == line.substr(0, 3)) {
      tex_coords[num_tex_coords++] = parse_vec2(line.substr(3));
    } else if ("vn " == line.substr(0, 3)) {
      normals[num_normals++] = parse_vec3(line.substr(3));
    } else if ("f " == line.substr(0, 2)) {
      auto f_line = line.substr(2);
      size_type f_curr = 0;
      size_type f_prev = 0;
      // delim by spaces
      while ((f_curr = f_line.find(" ", f_prev)) != std::string_view::npos) {
        auto f_len = f_curr - f_prev;
        auto face_tok = f_line.substr(f_prev, f_len);
        indices[mesh.m_num_vertices++] = parse_index(face_tok);
        f_prev += f_len + 1;
      }
      // do last token
      indices[mesh.m_num_vertices++] =
          parse_index(f_line.substr(f_prev, f_line.size() - f_prev - 1));
    }
    // TODO mtl parsing here
  }

  // TODO a fast way of detecting duplicate indices for ebo
  // TODO collect data on how big these files usually are
  mesh.mp_vertices = new Mesh::Vertex[mesh.m_num_vertices];

  for (unsigned int i = 0; i < mesh.m_num_vertices; i++) {
    auto index = indices[i];
    mesh.mp_vertices[i].m_position = positions[index.m_position_idx];
    mesh.mp_vertices[i].m_normal = normals[index.m_normal_idx];
    mesh.mp_vertices[i].m_tex_coord = tex_coords[index.m_tex_coord_idx];
  }

  mmapfile_unmap(file);
  */
  return mesh;
}

/**
 * ----------------------------------------------------------------------------
 */
void mesh_unload(struct mesh *mesh) {
  free(mesh->mp_vertices);
  mesh->mp_vertices = NULL;
}

/**
 * ----------------------------------------------------------------------------
 */
void gen_mesh_buffers(struct mesh *mesh) {
  glGenBuffers(1, &mesh->m_vbo);
  // glGenBuffers(1, &m_ebo);
  glGenVertexArrays(1, &mesh->m_vao);
  glBindVertexArray(mesh->m_vao);

  glBindBuffer(GL_ARRAY_BUFFER, mesh->m_vbo);
  glBufferData(GL_ARRAY_BUFFER, mesh->m_num_vertices * sizeof(struct vertex),
               (void *)(mesh->mp_vertices), GL_DYNAMIC_DRAW);

  /*
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_DYNAMIC_DRAW);
  */

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(offsetof(struct vertex, m_normal)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex),
                        (void *)(offsetof(struct vertex, m_tex_coord)));
  glEnableVertexAttribArray(2);
}

/**
 * ----------------------------------------------------------------------------
 */
void draw_mesh(struct mesh *mesh) {
  glBindVertexArray(mesh->m_vao);
  glDrawArrays(GL_TRIANGLES, 0, mesh->m_num_vertices);
  /*
  glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_INT,
                 static_cast<void *>(0));
  */
}
