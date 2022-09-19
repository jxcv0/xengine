#include "resourcesubsystem.h"

#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/texture.h>

#include <assimp/Importer.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "resource.h"
#include "stb_image.h"
#include "types.h"

/*------------------------------------------------------------------------------
 */
auto ResourceSubsystem::mesh_loaded(const char *filepath) const {
  return std::find_if(m_loaded_models.begin(), m_loaded_models.end(),
                      [=](const auto &r) { return r.filepath() == filepath; });
}

/*------------------------------------------------------------------------------
 */
auto ResourceSubsystem::material_loaded(const char *filepath) const {
  return std::find_if(m_loaded_materials.begin(), m_loaded_materials.end(),
                      [=](const auto &r) { return r.filepath() == filepath; });
}

/*------------------------------------------------------------------------------
 */
Resource<Model> ResourceSubsystem::load_model(const char *filepath) {
  auto it = mesh_loaded(filepath);
  if (m_loaded_models.end() != it) {
    return *it;
  }

  Assimp::Importer importer;
  auto flags = (aiProcess_Triangulate | aiProcess_FlipUVs |
                aiProcess_GenNormals | aiProcess_CalcTangentSpace);

  const aiScene *scene = importer.ReadFile(filepath, flags);

  if (nullptr == scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      nullptr == scene->mRootNode) {
    throw std::runtime_error(importer.GetErrorString());
  }
  // std::string path = filepath;
  // const auto dir = path.substr(0, path.find_last_of('/'));
  auto meshes = process_node(filepath, scene->mRootNode, scene);
  Resource<Model> new_resource(new Model{meshes}, filepath);
  m_loaded_models.push_back(new_resource);
  return new_resource;
}

/*------------------------------------------------------------------------------
 */
std::vector<Mesh> ResourceSubsystem::process_node(const char *filepath,
                                                  aiNode *node,
                                                  const aiScene *scene) {
  std::vector<Mesh> meshes;
  for (auto i = 0; i < node->mNumMeshes; ++i) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(process_mesh(filepath, mesh, scene));
  }

  for (auto i = 0; i < node->mNumChildren; ++i) {
    auto m = process_node(filepath, node->mChildren[i], scene);
    meshes.insert(meshes.end(), m.begin(), m.end());
  }

  return meshes;
}

/*------------------------------------------------------------------------------
 */
Mesh ResourceSubsystem::process_mesh(const char *filepath, aiMesh *mesh,
                                     const aiScene *scene) {
  std::vector<Vertex> vertices(mesh->mNumVertices);
  std::vector<uint32_t> indices(mesh->mNumFaces * 3);  // triangulated

  // vertices
  for (auto i = 0; i < mesh->mNumVertices; ++i) {
    Vertex vertex;
    vertex.m_position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y,
                                  mesh->mVertices[i].z);

    vertex.m_normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
                                mesh->mNormals[i].z);

    // assuming vertices only have one texture coordinate per vertex
    vertex.m_tex_coord =
        glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

    vertex.m_tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y,
                                 mesh->mTangents[i].z);

    vertex.m_bitangent = glm::vec3(
        mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);

    vertices.push_back(vertex);
  }

  // indices
  for (auto i = 0; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];
    for (auto j = 0; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }

  // materials
  aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
  load_materials(material, filepath);

  return Mesh{vertices, indices};
}

/*------------------------------------------------------------------------------
 */
Resource<Material> ResourceSubsystem::load_materials(aiMaterial *mat,
                                                     const char *filepath) {
  auto it = material_loaded(filepath);
  if (m_loaded_materials.end() != it) {
    return *it;
  }
  auto diff = load_textures(mat, aiTextureType_DIFFUSE);
  auto spec = load_textures(mat, aiTextureType_SPECULAR);
  auto norm = load_textures(mat, aiTextureType_HEIGHT);
  auto height = load_textures(mat, aiTextureType_AMBIENT);

  auto material = new Material();
  material->m_textures.insert(material->m_textures.end(), diff.begin(),
                              diff.end());
  material->m_textures.insert(material->m_textures.end(), spec.begin(),
                              spec.end());
  material->m_textures.insert(material->m_textures.end(), norm.begin(),
                              norm.end());
  material->m_textures.insert(material->m_textures.end(), height.begin(),
                              height.end());
  Resource<Material> resource(material, filepath);
  return resource;
}

/*------------------------------------------------------------------------------
 */
std::vector<Texture> ResourceSubsystem::load_textures(aiMaterial *mat,
                                                      aiTextureType type) {
  std::vector<Texture> textures;
  stbi_set_flip_vertically_on_load(true);
  for (auto i = 0; i < mat->GetTextureCount(type); ++i) {
    aiString aistr;
    mat->GetTexture(type, 0, &aistr);
    Texture texture;
    texture.mp_data = stbi_load(aistr.C_Str(), &texture.m_width,
                                &texture.m_height, &texture.m_num_channels, 0);
    if (nullptr == texture.mp_data) {
      // TODO load default.
    }

    switch (type) {
      case aiTextureType_DIFFUSE:
        texture.m_type = TextureType::diffuse;
        break;
      case aiTextureType_SPECULAR:
        texture.m_type = TextureType::specular;
        break;
      case aiTextureType_HEIGHT:
        texture.m_type = TextureType::normal;
        break;
      case aiTextureType_AMBIENT:
        texture.m_type = TextureType::height;
        break;
      default:
        texture.m_type = TextureType::diffuse;
        break;
    }

    textures.push_back(texture);
  }
  return textures;
}

/*------------------------------------------------------------------------------
 * TODO this should be elsewhere
void
Mesh::gen_buffers() {
  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);
  glGenBuffers(1, &m_EBO);

  glBindVertexArray(m_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
      m_vertices.data(), GL_STATIC_DRAW);

  // indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               m_indices.size() * sizeof(uint32_t),
               m_indices.data(), GL_STATIC_DRAW);

  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

  // texture coords
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      (void*)offsetof(Vertex, m_tex_coord));

  // vertex normals
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      (void*)offsetof(Vertex, m_normal));

  // tangent
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      (void*)offsetof(Vertex, m_tangent));

  // bitangent
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
      (void*)offsetof(Vertex, m_bitangent));

  // unbind
  glBindVertexArray(0);
}
*/

/*------------------------------------------------------------------------------
 */
