#include "resourcesubsystem.h"

#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/texture.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "types.h"
#include "resource.h"

/*------------------------------------------------------------------------------
 */
Resource<Model>
ResourceSubsystem::load_model(const char *filepath) {
  auto it = mesh_loaded(filepath);
  if(m_loaded_models.end() != it) {
    return *it;
  }

  Assimp::Importer importer;
  auto flags = (aiProcess_Triangulate |
                aiProcess_FlipUVs | 
                aiProcess_GenNormals);
  const aiScene *scene = importer.ReadFile(filepath, flags);

  if (nullptr == scene ||
      scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
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
std::vector<Mesh>
ResourceSubsystem::process_node(const char *filepath,
                                aiNode *node,
                                const aiScene *scene)
{
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
Mesh
ResourceSubsystem::process_mesh(const char *filepath,
                                aiMesh *mesh,
                                const aiScene *scene)
{
  std::vector<Vertex> vertices(mesh->mNumVertices);
  std::vector<uint32_t> indices(mesh->mNumFaces * 3); // triangulated

  // vertices
  for (auto i = 0; i < mesh->mNumVertices; ++i) {
    Vertex vertex;
    vertex.m_position = glm::vec3(mesh->mVertices[i].x,
                                  mesh->mVertices[i].y,
                                  mesh->mVertices[i].z);

    vertex.m_normal = glm::vec3(mesh->mNormals[i].x,
                                mesh->mNormals[i].y,
                                mesh->mNormals[i].z);

    // assuming vertices only have one texture coordinate per vertex
    vertex.m_tex_coord = glm::vec2(mesh->mTextureCoords[0][i].x,
                                   mesh->mTextureCoords[0][i].y);

    vertex.m_tangent = glm::vec3(mesh->mTangents[i].x,
                                 mesh->mTangents[i].y,
                                 mesh->mTangents[i].z);

    vertex.m_bitangent = glm::vec3(mesh->mBitangents[i].x,
                                   mesh->mBitangents[i].y,
                                   mesh->mBitangents[i].z);

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
Resource<Material>
ResourceSubsystem::load_materials(aiMaterial *mat, const char *filepath) {
  auto it = material_loaded(filepath);
  if(m_loaded_materials.end() != it) {
    return *it;
  }
  auto diff = load_texture(mat, aiTextureType_DIFFUSE);
  auto spec = load_texture(mat, aiTextureType_SPECULAR);
  auto height = load_texture(mat, aiTextureType_AMBIENT);
  Resource<Material> material(new Material{diff, spec, height}, filepath);
  return material;
}

/*------------------------------------------------------------------------------
 */
Texture
ResourceSubsystem::load_texture(aiMaterial *mat, aiTextureType type)
{
  stbi_set_flip_vertically_on_load(true);
  assert(1 == mat->GetTextureCount(type));
  aiString aistr;
  mat->GetTexture(type, 0, &aistr);
  Texture tex;
  tex.mp_data = stbi_load(
      aistr.C_Str(), &tex.m_width, &tex.m_height, &tex.m_num_channels, 0);
  if (nullptr == tex.mp_data) {
    // TODO load default.
  }
  return tex;
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

