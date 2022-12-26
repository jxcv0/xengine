#include "mesh.h"

#include <glad.h>
#include <unistd.h>

#include <cstddef>
#include <cstdlib>

#include "mmapfile.h"
#include "vec2.h"
#include "vec3.h"

/**
 * ----------------------------------------------------------------------------
 */
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

/**
 * ----------------------------------------------------------------------------
 */
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

/**
 * ----------------------------------------------------------------------------
 */
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

/**
 * ----------------------------------------------------------------------------
 */
Mesh load_mesh(const char *filepath) {
  (void) filepath;
  Mesh mesh;
  /*
  using size_type = std::string_view::size_type;

  const MmapFile file(filepath);
  auto view = file.view();

  // count up how much memory to allocate.
  unsigned int num_positions = 0;
  unsigned int num_tex_coords = 0;
  unsigned int num_normals = 0;
  mesh.m_num_vertices = 0;
  size_type curr = 0;
  size_type prev = 0;
  while ((curr = view.find('\n', prev)) != std::string_view::npos) {
    auto len = curr - prev + 1;
    auto line = view.substr(prev, len);
    prev += len;
    if ("v " == line.substr(0, 2)) {
      num_positions++;
    } else if ("vt " == line.substr(0, 3)) {
      num_tex_coords++;
    } else if ("vn " == line.substr(0, 3)) {
      num_normals++;
    } else if ("f " == line.substr(0, 2)) {
      mesh.m_num_vertices += 3;  // 3 per line
    }
  }

  Vec3 positions[num_positions];
  Vec3 normals[num_normals];
  Vec2 tex_coords[num_tex_coords];
  Mesh::Index indices[mesh.m_num_vertices];

  // TODO buffer overrun here
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
  */
  return mesh;
}

/**
 * ----------------------------------------------------------------------------
 */
void gen_mesh_buffers(Mesh *mesh) {
  glGenBuffers(1, &mesh->m_vbo);
  // glGenBuffers(1, &m_ebo);
  glGenVertexArrays(1, &mesh->m_vao);
  glBindVertexArray(mesh->m_vao);

  glBindBuffer(GL_ARRAY_BUFFER, mesh->m_vbo);
  glBufferData(GL_ARRAY_BUFFER, mesh->m_num_vertices * sizeof(Mesh::Vertex),
               static_cast<void *>(mesh->mp_vertices), GL_DYNAMIC_DRAW);

  /*
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_DYNAMIC_DRAW);
  */

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex),
                        reinterpret_cast<void *>(0));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
      1, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex),
      reinterpret_cast<void *>(offsetof(Mesh::Vertex, m_normal)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(
      2, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex),
      reinterpret_cast<void *>(offsetof(Mesh::Vertex, m_tex_coord)));
  glEnableVertexAttribArray(2);
}

/**
 * ----------------------------------------------------------------------------
 */
void draw_mesh(Mesh *mesh) {
  glBindVertexArray(mesh->m_vao);
  glDrawArrays(GL_TRIANGLES, 0, mesh->m_num_vertices);
  /*
  glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_INT,
                 static_cast<void *>(0));
  */
}
