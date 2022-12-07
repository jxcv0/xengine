#ifndef IMPORT_H_
#define IMPORT_H_

#include <filesystem>
#include <iterator>
#include <memory>
#include <string_view>

#include "material.h"
#include "mesh.h"
#include "mmapfile.h"
#include "vec2.h"
#include "vec3.h"

namespace xen {

auto load_texture(const std::filesystem::path &filepath);

/**
 * @brief Unspecialized import function throws runtime error.
 */
template <typename T, template <typename> typename Alloc = std::allocator>
void import([[maybe_unused]] T *dest,
            [[maybe_unused]] const std::filesystem::path &filepath,
            [[maybe_unused]] Alloc<T> &alloc) {
  throw std::runtime_error("no implementation for importing type");
}

// TODO mesh and material to be treated as separate components?

/**
 * @brief Function specialization for importing a mesh from a wavefront .obj
 *        file. Meshes MUST be triangulated.
 *
 * @param mesh A pointer to the destination mesh.
 * @param filepath The path to the file to import.
 */
template <template <typename> typename Alloc = std::allocator>
void import(Material *mat, const std::filesystem::path &filepath,
            Alloc<Mesh> &alloc) {
  if (filepath.extension() != ".mtl") {
    throw std::runtime_error("file extension not supported");
  }
  // TODO
  (void)mat;
  (void)alloc;
}

/**
 * @brief Parse a whitespace separated string of floats and add them to the
 *        mesh object.
 *     TODO this comment
 */
Vec3 parse_vec3(const std::string_view &sv);

Vec2 parse_vec2(const std::string_view &sv);

Mesh::Index parse_index(const std::string_view &sv);

/**
 * @brief Function specialization for importing a material from a wavefront
 *        .mtl file.
 *
 * @param texture A pointer to the destination material.
 * @param filepath The path to the file to import.
 */
template <template <typename> typename Alloc>
void import(Mesh *mesh, const std::filesystem::path &filepath,
            [[maybe_unused]] Alloc<Mesh> &alloc) {
  using size_type = std::string_view::size_type;
  // c++ way of doing things but i dont like it.
  // Alloc<Mesh>::rebind<void>::other needs to be specialized
  using VoidAlloc =
      typename std::allocator_traits<Alloc<Mesh>>::template rebind_alloc<Vec3>;
  if (filepath.extension() != ".obj") {
    throw std::runtime_error("file extension not supported");
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

  mesh->m_num_positions = 0;
  mesh->m_num_normals = 0;
  mesh->m_num_tex_coords = 0;
  mesh->m_num_indices = 0;

  auto positions_size = num_positions * sizeof(Vec3);
  auto tex_coords_size = num_tex_coords * sizeof(Vec2);
  auto normals_size = num_normals * sizeof(Vec3);
  auto faces_size = num_faces * sizeof(Mesh::Index) * 3;  // 3 per line
  auto mem_size = positions_size + tex_coords_size + normals_size + faces_size;

  // pointers into memory block
  VoidAlloc va;
  auto *memory_block = va.allocate(mem_size);
  mesh->mp_memory_block = memory_block;

  // TODO get rid of these C casts.
  mesh->mp_positions = reinterpret_cast<Vec3 *>(memory_block);
  mesh->mp_normals = reinterpret_cast<Vec3 *>(memory_block + positions_size);
  mesh->mp_tex_coords =
      reinterpret_cast<Vec2 *>(memory_block + positions_size + normals_size);
  mesh->mp_indices = reinterpret_cast<Mesh::Index *>(
      memory_block + positions_size + normals_size + tex_coords_size);

  // the actual parsing
  curr = 0;
  prev = 0;
  while ((curr = view.find('\n', prev)) != std::string_view::npos) {
    auto len = curr - prev + 1;
    auto line = view.substr(prev, len);
    prev += len;

    if ("v " == line.substr(0, 2)) {
      mesh->mp_positions[mesh->m_num_positions++] = parse_vec3(line.substr(2));
    } else if ("vt " == line.substr(0, 3)) {
      mesh->mp_tex_coords[mesh->m_num_tex_coords++] =
          parse_vec2(line.substr(3));
    } else if ("vn " == line.substr(0, 3)) {
      mesh->mp_normals[mesh->m_num_normals++] = parse_vec3(line.substr(3));
    } else if ("f " == line.substr(0, 2)) {
      mesh->mp_indices[mesh->m_num_indices++] = parse_index(line.substr(2));
    }
  }
}
}  // namespace xen

#endif  // IMPORT_H_
