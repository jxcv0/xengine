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

static auto parse_face(std::stringstream& line) {

}

template <>
void import_impl::import(Mesh *mesh, const std::filesystem::path &filepath) {

  if (filepath.extension() != ".obj") {
    throw std::runtime_error("file extension not supported");
  }

  std::ifstream filestream(filepath);

  // temporary storage for parsed data
  std::vector<Vec3> positions;
  std::vector<Vec2> tex_coords;
  std::vector<Vec3> normals;
  // std::vector<Material> materials;
  std::vector<Index> indices;

  for (std::string line; std::getline(filestream, line);) {
    std::stringstream linestream(line);
    std::string line_token;
    linestream >> line_token;
    if (line_token == "#") {
      // skip comments - kinda redundant optimization
      continue;
    } else if (line_token == "mtllib") {
      auto mtllib = parse_mtllib(mesh, filepath, linestream);
    } else if (line_token == "v") {
      positions.push_back(parse_vertex(linestream));
    } else if (line_token == "vt") {
      tex_coords.push_back(parse_tex_coords(linestream));
    } else if (line_token == "vn") {
      normals.push_back(parse_normal(linestream));
    } else if (line_token == "usemtl") {
      // TODO use materials 
    } else if (line_token == "f") {
      // TODO
      // indices.push_back(parse_face(linestream));
      std::cout << "face\n";
    }
  }
}
