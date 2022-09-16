#include "model.h"

#include <stdexcept>
#include <vector>
#include <map>

#include <assimp/material.h>
#include <assimp/texture.h>
#include <cstdint>
#include <cstring>
#include <glm/fwd.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/*------------------------------------------------------------------------------
 */
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

/*------------------------------------------------------------------------------
 */
uint32_t
MeshUtils::load_texture(const char* path) {
  uint32_t texture_id;
  glGenTextures(1, &texture_id);

  int width, height, num_comp;
  unsigned char* data = stbi_load(path, &width, &height, &num_comp, 0);

  if (data) {
    GLenum format;
    if (num_comp == 1) {
      format = GL_RED;
    } else if (num_comp == 3) {
      format = GL_RGB;
    } else if (num_comp == 4) {
      format = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(
        GL_TEXTURE_2D,
        0, format,
        width, height,
        0, format,
        GL_UNSIGNED_BYTE,
        data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  } else {
    std::cout << "Unable to load texture from " << path << "\n";
  }

  stbi_image_free(data);
  return texture_id;
}

/*------------------------------------------------------------------------------
 */
void
Model::load(const char *filepath) {
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
  std::string path = filepath;
  const auto dir = path.substr(0, path.find_last_of('/'));
  process_node(dir.c_str(), scene->mRootNode, scene);
}

/*------------------------------------------------------------------------------
 */
void
Model::process_node(const char *dir, aiNode *node, const aiScene *scene) {
  for (auto i = 0; i < node->mNumMeshes; ++i) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    process_mesh(dir, mesh, scene);
  }

  for (auto i = 0; i < node->mNumChildren; ++i) {
    process_node(dir, node->mChildren[i], scene);
  }
}

/*------------------------------------------------------------------------------
 */
void
Model::process_mesh(const char *dir, aiMesh *mesh, const aiScene *scene) {
  std::vector<Vertex> vertices(mesh->mNumVertices);
  std::vector<uint32_t> indices(mesh->mNumFaces * 3); // triangulated
  std::vector<Texture> textures(3); // TODO how to find number of textures?

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
  auto diffuse = load_material(material, aiTextureType_DIFFUSE, "texture_diffuse");
  auto specular = load_material(material, aiTextureType_SPECULAR, "texture_specular");
  auto height = load_material(material, aiTextureType_AMBIENT, "texture_height");
  textures.insert(textures.end(), diffuse.begin(), diffuse.end());

  Mesh m(vertices, indices, textures);
  m_meshes.push_back(m);
}

/*------------------------------------------------------------------------------
 */
std::vector<Texture>
Model::load_material(aiMaterial *mat, aiTextureType type, const char *name) {
  // this is just terrible design
  // this needs to be handled by a resource manager
  // also materials should be a separate component
  std::vector<Texture> textures;
  for (auto i = 0; i < mat->GetTextureCount(type); ++i) {
    aiString aistr;
    mat->GetTexture(type, i, &aistr);
    Texture texture;
    texture.m_id = MeshUtils::load_texture(aistr.C_Str());
    textures.push_back(texture);
  }
  return textures;
}

