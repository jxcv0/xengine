#ifndef MESH_H_
#define MESH_H_

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <limits>

#include "glad.h"
#include "mmapfile.h"
#include "vec2.h"
#include "vec3.h"

/**
 * @brief Stored mesh data. This class is used to load and
 *        unload from a file using an allocator supplied to it.
 */
class Mesh {
  struct Index {
    unsigned int m_position_idx;
    unsigned int m_tex_coord_idx;
    unsigned int m_normal_idx;
  };

 public:
  /**
   * @brief Load mesh data from an obj file.
   *
   * @tparam Alloc The allocator class type to use to allocate mesh memory.
   * @param filepath The filepath to load data from.
   * @param alloc A reference to the allocator to use.
   */
  void load(const std::filesystem::path &filepath) {
    using size_type = std::string_view::size_type;

    if (filepath.extension() != ".obj") {
      mp_memory_block = nullptr;
      return;
    }

    const MmapFile file(filepath);
    auto view = file.view();

    // count up how much memory to allocate.
    int num_positions = 0;
    int num_tex_coords = 0;
    int num_normals = 0;
    int num_faces = 0;
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
        num_faces++;
      }
    }

    m_num_positions = 0;
    m_num_normals = 0;
    m_num_tex_coords = 0;
    m_num_indices = 0;

    auto positions_size = num_positions * sizeof(Vec3);
    auto tex_coords_size = num_tex_coords * sizeof(Vec2);
    auto normals_size = num_normals * sizeof(Vec3);
    auto faces_size = num_faces * sizeof(Mesh::Index) * 3;  // 3 per line
    auto mem_size =
        positions_size + tex_coords_size + normals_size + faces_size;

    // TODO need an allocator here.
    // pointers into memory block
    auto *mp_memory_block = std::malloc(mem_size);
    auto addr = reinterpret_cast<std::uintptr_t>(mp_memory_block);

    // TODO get rid of these C casts.
    mp_positions = reinterpret_cast<Vec3 *>(addr);
    mp_normals = reinterpret_cast<Vec3 *>(addr + positions_size);
    mp_tex_coords =
        reinterpret_cast<Vec2 *>(addr + positions_size + normals_size);
    mp_indices = reinterpret_cast<Mesh::Index *>(
        addr + positions_size + normals_size + tex_coords_size);

    // the actual parsing
    curr = 0;
    prev = 0;
    while ((curr = view.find('\n', prev)) != std::string_view::npos) {
      auto len = curr - prev + 1;
      auto line = view.substr(prev, len);
      prev += len;

      if ("v " == line.substr(0, 2)) {
        mp_positions[m_num_positions++] = parse_vec3(line.substr(2));
      } else if ("vt " == line.substr(0, 3)) {
        mp_tex_coords[m_num_tex_coords++] = parse_vec2(line.substr(3));
      } else if ("vn " == line.substr(0, 3)) {
        mp_normals[m_num_normals++] = parse_vec3(line.substr(3));
      } else if ("f " == line.substr(0, 2)) {
        auto f_line = line.substr(2);
        size_type f_curr = 0;
        size_type f_prev = 0;
        // delim by spaces
        while ((f_curr = f_line.find(" ", f_prev)) != std::string_view::npos) {
          auto f_len = f_curr - f_prev;
          auto face_tok = f_line.substr(f_prev, f_len);
          mp_indices[m_num_indices++] = parse_index(face_tok);
          f_prev += f_len + 1;
        }
        // do last token
        mp_indices[m_num_indices++] =
            parse_index(f_line.substr(f_prev, f_line.size() - f_prev - 1));
      }
      // TODO mtl parsing here
    }
  }

  /**
   * @brief Unload the memory used by the Mesh
   *
   * @tparam Del The type of the deleter class.
   * @param deleter The deleter class used to free the memory.
   */
  void unload() { std::free(mp_memory_block); }

  /**
   * @brief Check if the Mesh is currently storing data.
   *
   * @return bool true if this Mesh points to usable memory.
   */
  auto in_use() { return mp_memory_block != nullptr; }

  /**
   * @brief Generate gl buffers for this mesh.
   */
  void gen_buffers() {
    auto buffsize = m_num_indices * 8;
    float data[buffsize];  // 8 floats per index
    float indices[m_num_indices];

    for (unsigned int i = 0, j = 0; i < m_num_indices; i++) {
      // TODO a fast way of detecting duplicate indices for ebo
      indices[i] = i;
      auto index = mp_indices[i];
      data[j++] = mp_positions[index.m_position_idx].x();
      data[j++] = mp_positions[index.m_position_idx].y();
      data[j++] = mp_positions[index.m_position_idx].z();
      data[j++] = mp_normals[index.m_normal_idx].x();
      data[j++] = mp_normals[index.m_normal_idx].y();
      data[j++] = mp_normals[index.m_normal_idx].z();
      data[j++] = mp_tex_coords[index.m_tex_coord_idx].x();
      data[j++] = mp_tex_coords[index.m_tex_coord_idx].y();
    }

    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_DYNAMIC_DRAW);

    auto stride = 8 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride,
                          reinterpret_cast<void *>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
                          reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride,
                          reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
  }

  void draw() {
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_INT, (void *)0);
  }

#ifndef MESH_GTEST
 private:
#endif
  /**
   * @brief Parse a string into a Vec3
   */
  Vec3 parse_vec3(const std::string_view &sv);

  /**
   * @brief Parse a string into a Vec2
   */
  Vec2 parse_vec2(const std::string_view &sv);

  /**
   * @brief Parse a string into an Index
   */
  Index parse_index(const std::string_view &sv);

  unsigned int m_vbo;
  unsigned int m_vao;
  unsigned int m_ebo;
  unsigned int m_num_positions = 0;
  unsigned int m_num_normals = 0;
  unsigned int m_num_tex_coords = 0;
  unsigned int m_num_indices = 0;
  void *mp_memory_block = nullptr;
  Vec3 *mp_positions = nullptr;
  Vec3 *mp_normals = nullptr;
  Vec2 *mp_tex_coords = nullptr;
  Index *mp_indices = nullptr;
};

#endif  // MESH_H_
