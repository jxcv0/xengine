#ifndef MESH_H_
#define MESH_H_

#include <cstdint>
#include <filesystem>
#include <memory>

#include "mmapfile.h"
#include "vec2.h"
#include "vec3.h"

/**
 * @brief Manages memory storing. This class is used to load and
 *        unload from a file.
 */
class Mesh {
  struct Index {
    unsigned int m_position_idx;
    unsigned int m_tex_coord_idx;
    unsigned int m_normal_idx;
  };

 public:
  template <template <typename> typename Alloc = std::allocator>
  void load(const std::filesystem::path &filepath, Alloc<void> &alloc) {
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

    // pointers into memory block
    auto *mp_memory_block = alloc.allocate(mem_size);
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

  template <template <typename> typename Del>
  void unload(Del<void> &deleter) {
    auto positions_size = m_num_positions * sizeof(Vec3);
    auto tex_coords_size = m_num_tex_coords * sizeof(Vec2);
    auto normals_size = m_num_normals * sizeof(Vec3);
    auto indices_size = m_num_indices * sizeof(Mesh::Index);
    auto mem_size =
        positions_size + tex_coords_size + normals_size + indices_size;
    deleter.deallocate(mp_indices, mem_size);
  }

#ifndef MESH_GTEST
 private:
#endif
  Vec3 parse_vec3(const std::string_view &sv);

  Vec2 parse_vec2(const std::string_view &sv);

  Index parse_index(const std::string_view &sv);

  void *mp_memory_block;
  Vec3 *mp_positions;
  Vec3 *mp_normals;
  Vec2 *mp_tex_coords;
  Index *mp_indices;
  unsigned int m_num_positions;
  unsigned int m_num_normals;
  unsigned int m_num_tex_coords;
  unsigned int m_num_indices;
};

#endif  // MESH_H_
