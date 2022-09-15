#include "model.h"

#include <assimp/texture.h>
#include <cstdint>
#include <stdexcept>
#include <vector>

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
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
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
  auto flags = (aiProcess_Triangulate | aiProcess_FlipUVs);
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

  for (auto i = 0; i < mesh->mNumVertices; ++i) {
    Vertex vertex;
    // TODO process
    vertices.push_back(vertex);
  }

  // TODO process indices
  
  if (mesh->mMaterialIndex >= 0) {
    // TODO process materials
  }

  Mesh m(vertices, indices, textures);
  m_meshes.push_back(m);
}

