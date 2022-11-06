#include "importer.h"

#include <filesystem>
#include <fstream>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

static const auto POST_PROCESS = aiProcess_Triangulate | aiProcess_FlipUVs |
                          aiProcess_CalcTangentSpace;

template <>
void import_impl::import(Texture *texture,
                         const std::filesystem::path &filepath) {
  texture->mp_data = stbi_load(filepath.c_str(), &texture->m_width,
                               &texture->m_height, &texture->m_num_channels, 0);
}

template<>
void import_impl::import(Model *model, const std::filesystem::path& filepath) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(filepath.c_str(), POST_PROCESS);
  // TODO ...
}
