#include <assert.h>
#include <assimp/cimport.h>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assets.h"

char directory[256];

struct vertex *out_vertices = NULL;
uint32_t *out_indices = NULL;
uint32_t out_num_vertices = 0;
uint32_t out_num_indices = 0;

FILE *file;

void process_node(struct aiNode *node, const struct aiScene *scene);
void process_mesh(struct aiMesh *mesh, const struct aiScene *scene);
char *process_material(struct aiMaterial *mat, enum aiTextureType);
char *diffuse_name = NULL;
char *normal_name = NULL;
char *roughness_name = NULL;
char *displacement_name = NULL;

/**
 * @brief Load a file using assimp and save it as a .geom and a .material file.
 * A geom file may only contain the definition of a single geometry (a single
 * group of vertices).
 */
int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("usage:\n\tconverter <in file> <out file>\n");
    return 0;
  }

  file = fopen(argv[2], "wb+");
  if (file == NULL) {
    perror("fopen");
  }

  const char *dir = strrchr(argv[1], '/');
  size_t n = dir - argv[1];
  strncpy(directory, argv[1], n);
  directory[n] = 0;
  printf("Loading from directory %s\n", directory);

  const struct aiScene *scene =
      aiImportFile(argv[1], aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                                aiProcess_JoinIdenticalVertices);
  if (scene == NULL) {
    fprintf(stderr, "%s\n", aiGetErrorString());
    return 0;
  }

  process_node(scene->mRootNode, scene);

  printf("Writing to file %s.\n", argv[2]);

  // write out
  fprintf(file, "VERTICES %d\n", out_num_vertices);
  fwrite(out_vertices, sizeof(struct vertex), out_num_vertices, file);
  fprintf(file, "\n");

  fprintf(file, "INDICES %d\n", out_num_indices);
  fwrite(out_indices, sizeof(uint32_t), out_num_indices, file);
  fprintf(file, "\n");

  // not loading these here
  /*
  fprintf(file, "DIFFUSE ");
  fprintf(file, "%s\n", diffuse_name);

  fprintf(file, "NORMAL ");
  fprintf(file, "%s\n", normal_name);

  fprintf(file, "ROUGHNESS ");
  fprintf(file, "%s\n", roughness_name);

  fprintf(file, "DISPLACEMENT ");
  fprintf(file, "%s\n", displacement_name);
  */

  aiReleaseImport(scene);
  fclose(file);
  return 0;
}

/**
 * ============================================================================
 */
void process_node(struct aiNode *node, const struct aiScene *scene) {
  if (node->mNumMeshes > 1) {
    fprintf(stderr, "Multiple meshes not supported. Exiting...\n");
    exit(EXIT_FAILURE);
  }

  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    struct aiMesh *m = scene->mMeshes[node->mMeshes[i]];
    process_mesh(m, scene);
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    process_node(node->mChildren[i], scene);
  }
}

/**
 * ============================================================================
 */
void process_mesh(struct aiMesh *mesh, const struct aiScene *scene) {
  out_num_vertices = mesh->mNumVertices;
  out_num_indices = mesh->mNumFaces * 3;
  size_t vertices_size = sizeof(struct vertex) * out_num_vertices;
  size_t indices_size = sizeof(uint32_t) * out_num_indices;
  struct vertex *vertices = malloc(vertices_size);
  uint32_t *indices = malloc(indices_size);

  printf("Loading mesh: %s.\n", mesh->mName.data);
  printf("Copying %d vertices.\n", mesh->mNumVertices);

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    struct vertex *v = &vertices[i];

    v->m_position[0] = mesh->mVertices[i].x;
    v->m_position[1] = mesh->mVertices[i].y;
    v->m_position[2] = mesh->mVertices[i].z;

    v->m_normal[0] = mesh->mNormals[i].x;
    v->m_normal[1] = mesh->mNormals[i].y;
    v->m_normal[2] = mesh->mNormals[i].z;

    v->m_tangent[0] = mesh->mTangents[i].x;
    v->m_tangent[1] = mesh->mTangents[i].y;
    v->m_tangent[2] = mesh->mTangents[i].z;

    v->m_bitangent[0] = mesh->mBitangents[i].x;
    v->m_bitangent[1] = mesh->mBitangents[i].y;
    v->m_bitangent[2] = mesh->mBitangents[i].z;

    if (mesh->mTextureCoords[0] != 0) {
      v->m_tex_coord[0] = mesh->mTextureCoords[0][i].x;
      v->m_tex_coord[1] = mesh->mTextureCoords[0][i].y;
    } else {
      v->m_tex_coord[0] = 0;
      v->m_tex_coord[1] = 0;
    }
  }

  printf("Copying indices from %d faces.\n", mesh->mNumFaces);
  for (unsigned int i = 0, j = 0; i < mesh->mNumFaces; i++) {
    struct aiFace face = mesh->mFaces[i];
    for (unsigned int k = 0; k < face.mNumIndices; k++) {
      indices[j++] = face.mIndices[k];
    }
  }

  (void)scene;
  // hot loading materials here
  /*
  printf("Loading materials.\n");
  struct aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
  diffuse_name = process_material(material, aiTextureType_DIFFUSE);
  roughness_name = process_material(material, aiTextureType_SHININESS); // is
  this correct? normal_name = process_material(material, aiTextureType_HEIGHT);
  // should this be HEIGHT? displacement_name = process_material(material,
  aiTextureType_DISPLACEMENT);
  */

  out_vertices = vertices;
  out_indices = indices;
}

char *process_material(struct aiMaterial *mat, enum aiTextureType type) {
  if (mat != NULL) {
    unsigned int tex_count = aiGetMaterialTextureCount(mat, type);
    for (unsigned int i = 0; i < tex_count; i++) {
      struct aiString str;
      int res = aiGetMaterialString(mat, AI_MATKEY_TEXTURE(type, i), &str);
      if (res != AI_SUCCESS) {
        fprintf(stderr, "error while getting texture\n");
        exit(0);
      }

      char *texture_filename = malloc(str.length + 1);
      strncpy(texture_filename, str.data, str.length);
      texture_filename[str.length] = 0;
      printf("Found texture %s.\n", texture_filename);
      return texture_filename;
    }
  }
  return NULL;
}
