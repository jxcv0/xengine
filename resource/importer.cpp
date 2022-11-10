#include "importer.h"

#include <stb_image.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <vec3.h>

template <>
void import_impl::import(Texture *texture,
                         const std::filesystem::path &filepath) {
  texture->mp_data = stbi_load(filepath.c_str(), &texture->m_width,
                               &texture->m_height, &texture->m_num_channels, 0);
}

// parse mtllib line of .obj file
static auto parse_mtllib(Mesh *mesh, std::filesystem::path filepath,
                         std::stringstream &line) {
  std::string str;
  line >> str;

  filepath.replace_filename(str);
  std::ifstream mtllib_stream(filepath);
  // TODO parse .mtl file
  return nullptr;
}

// parse vertex ("v") line of .obj file
static auto parse_vertex(std::stringstream& line) {
  float x, y, z;
  line >> x >> y >> z;
  return Vec3(x, y, z);
}

// parse texture coordinates ("vt") line of .obj file
static auto parse_tex_coords(std::stringstream& line) {
  float x, y;
  line >> x >> y;
  return Vec2(x, y);
}

// parse texture coordinates ("vt") line of .obj file
static auto parse_normal(std::stringstream& line) {
  float x, y, z;
  line >> x >> y >> z;
  return Vec3(x, y, z);
}

struct Index {
  int m_vertex_index;
  int m_normal;
  int m_tex_coord_index;
};

// parse face ("f") line of .obj file
static auto parse_face(std::stringstream& line) {
  int vertex, normal, tex_coords;
  line >> vertex >> normal >> tex_coords;
  return Index{vertex--, normal--, tex_coords--};
}

template <>
void import_impl::import(Mesh *mesh, const std::filesystem::path &filepath) {
  if (filepath.extension() != ".obj") {
    throw std::runtime_error("file extension not supported");
  }

  std::ifstream filestream(filepath);

  // TODO ...
}
