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
#include <libgen.h>

#include "assets.h"

#define MAX_MESHES 32
#define MAX_TEXTURES 3

char directory[256];

unsigned int num_meshes = 0;
struct vertex *out_vertices[MAX_MESHES] = {0};
uint32_t *out_indices[MAX_MESHES] = {0};
uint32_t out_num_vertices[MAX_MESHES] = {0};
uint32_t out_num_indices[MAX_MESHES] = {0};
char *out_diff_names[MAX_MESHES] = {0};
char *out_spec_names[MAX_MESHES] = {0};

FILE *file;

void process_node(struct aiNode *node, const struct aiScene *scene);
void process_mesh(struct aiMesh *mesh, const struct aiScene *scene);
char *process_material(struct aiMaterial *mat, enum aiTextureType);

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("usage:\n\tconverter <in file> <out file>\n");
    return 0;
  }

  file = fopen(argv[2], "wb+");
  if (file == NULL) {
    perror("fopen");
  }

  const struct aiScene *scene =
      aiImportFile(argv[1], aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                                aiProcess_JoinIdenticalVertices);
  if (scene == NULL) {
    fprintf(stderr, "%s\n", aiGetErrorString());
    return 0;
  }

  process_node(scene->mRootNode, scene);

  printf("Writing to file \"%s\".\n", argv[2]);

  // write to file
  fprintf(file, "{\n");
  fprintf(file, "\t\"num_meshes\": \"%u\",\n", num_meshes);
  fprintf(file, "\t\"meshes\": [\n");
  for (unsigned int i = 0; i < num_meshes; i++) {
    fprintf(file, "\t\t{\n");
    fprintf(file, "\t\t\t\"num_vertices\": \"%u\",\n", out_num_vertices[i]);
    fprintf(file, "\t\t\t\"vertices_data\": \"");
    fwrite(out_vertices[i], sizeof(struct vertex), out_num_vertices[i], file);
    fprintf(file, "\"\n");
    fprintf(file, "\t\t\t\"num_indices\": \"%u\",\n", out_num_indices[i]);
    fprintf(file, "\t\t\t\"indices_data\": \"");
    fwrite(out_indices[i], sizeof(uint32_t), out_num_indices[i], file);
    fprintf(file, "\"\n");

    fprintf(file, "<count>");
    fprintf(file, "%u", out_num_vertices[i]);
    fprintf(file, "</count>");
    fprintf(file, "<data>");
    fprintf(file, "</data>");
    fprintf(file, "</vertices>");

    fprintf(file, "<indices>");
    fprintf(file, "<count>");
    fprintf(file, "%u", out_num_indices[i]);
    fprintf(file, "</count>");
    fprintf(file, "<data>");
    fwrite(out_indices[i], sizeof(uint32_t), out_num_indices[i], file);
    fprintf(file, "</data>");
    fprintf(file, "</indices>");

    fprintf(file, "<diffuse>");
    if (out_diff_names[i] != NULL) {
      fprintf(file, "%s", out_diff_names[i]);
    }
    fprintf(file, "</diffuse>");

    fprintf(file, "<specular>");
    if (out_spec_names[i] != NULL) {
      fprintf(file, "%s", out_spec_names[i]);
    }
    fprintf(file, "</specular>");

    fprintf(file, "\t\t\"]\n");
    fprintf(file, "\t\t}\n");
  }

  fprintf(file, "\t]\n");
  fprintf(file, "}");

  aiReleaseImport(scene);
  fclose(file);
  return 0;
}

void process_node(struct aiNode *node, const struct aiScene *scene) {
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    struct aiMesh *m = scene->mMeshes[node->mMeshes[i]];
    process_mesh(m, scene);
  }

  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    process_node(node->mChildren[i], scene);
  }
}

void process_mesh(struct aiMesh *mesh, const struct aiScene *scene) {
  assert(num_meshes < MAX_MESHES);
  out_num_vertices[num_meshes] = mesh->mNumVertices;
  out_num_indices[num_meshes] = mesh->mNumFaces * 3;
  size_t vertices_size = sizeof(struct vertex) * out_num_vertices[num_meshes];
  size_t indices_size = sizeof(uint32_t) * out_num_indices[num_meshes];
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

    if (mesh->mTextureCoords[0] != 0) {
      // only one texture per mesh
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

  printf("Loading materials.\n");
  struct aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
  out_diff_names[num_meshes] = process_material(material, aiTextureType_DIFFUSE);
  out_spec_names[num_meshes] = process_material(material, aiTextureType_SPECULAR);
  // out_norm_name[num_meshes] = process_material(material, aiTextureType_HEIGHT);

  out_vertices[num_meshes] = vertices;
  out_indices[num_meshes] = indices;

  num_meshes++;
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
