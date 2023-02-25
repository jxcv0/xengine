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
#include <argp.h>

#include "assets.h"

/**
 * @brief This is the file that contains the relative filepaths of all the
 * meshes, textures and animations that make up the model.
 */
FILE *model_file;
char *model_name;
char *in_file;

// char *diffuse_name = NULL;
// char *normal_name = NULL;
// char *roughness_name = NULL;
// char *displacement_name = NULL;

void process_node(struct aiNode *node, const struct aiScene *scene);
void process_mesh(struct aiMesh *mesh, const struct aiScene *scene);
char *process_material(struct aiMaterial *mat, enum aiTextureType);

// command line options
static struct argp_option options[] = {
{"infile", 'i', "input file", 0, "The file to convert", 0},
{"name", 'n', "model name", 0, "The name of the resulting model", 0},
{0}
};

static error_t parse_opt(int key, char *argv, struct argp_state *state) {
  switch (key) {
    case 'i':
      in_file = argv;
      break;
    case 'n':
      model_name = argv;
      break;
    case ARGP_KEY_ARG:
      if (state->arg_num > 2) {
        argp_usage(state);
      }
      exit(EXIT_FAILURE);
      break;
    case ARGP_KEY_END:
      if (state->arg_num < 2) {
        argp_usage(state);
      }
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  struct argp parser = {0};
  parser.options = options;
  parser.parser = parse_opt;
  argp_parse(&parser, argc, argv, 0, 0, 0);

  model_file = fopen(in_file, "wb+");
  const struct aiScene *scene =
      aiImportFile(argv[1], aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                                aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

  if (scene == NULL) {
    fprintf(stderr, "%s\n", aiGetErrorString());
    return 0;
  }

  process_node(scene->mRootNode, scene);

  aiReleaseImport(scene);
  return 0;
}

/**
 * ============================================================================
 */
void process_node(struct aiNode *node, const struct aiScene *scene) {
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
  uint32_t num_vertices = mesh->mNumVertices;
  uint32_t num_indices = mesh->mNumFaces * 3;
  size_t vertices_size = sizeof(struct vertex) * num_vertices;
  size_t indices_size = sizeof(uint32_t) * num_indices;
  struct vertex *vertices = malloc(vertices_size);
  uint32_t *indices = malloc(indices_size);

  char *mesh_name = mesh->mName.data;
  printf("Loading mesh: %s.\n", mesh_name);
  printf("Copying %d vertices.\n", mesh->mNumVertices);

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    struct vertex *v = &vertices[i];

    v->m_position[0] = mesh->mVertices[i].x;
    v->m_position[1] = mesh->mVertices[i].y;
    v->m_position[2] = mesh->mVertices[i].z;

    assert(mesh->mTangents != NULL); // if no texture is specified in the source file.
    v->m_tangent[0] = mesh->mTangents[i].x;
    v->m_tangent[1] = mesh->mTangents[i].y;
    v->m_tangent[2] = mesh->mTangents[i].z;

    v->m_bitangent[0] = mesh->mBitangents[i].x;
    v->m_bitangent[1] = mesh->mBitangents[i].y;
    v->m_bitangent[2] = mesh->mBitangents[i].z;

    v->m_normal[0] = mesh->mNormals[i].x;
    v->m_normal[1] = mesh->mNormals[i].y;
    v->m_normal[2] = mesh->mNormals[i].z;

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

  /*
  printf("Loading materials.\n");
  struct aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
  diffuse_name = process_material(material, aiTextureType_DIFFUSE);
  roughness_name = process_material(material, aiTextureType_SHININESS); // is
  this correct? normal_name = process_material(material, aiTextureType_HEIGHT);
  // should this be HEIGHT? displacement_name = process_material(material,
  aiTextureType_DISPLACEMENT);
  */

  char filename[64] = {0};
  size_t mesh_name_len = strlen(mesh_name);
  strncpy(filename, mesh_name, mesh_name_len);
  strncpy(&filename[mesh_name_len], ".geom", 6);

  FILE *file = fopen(filename, "wb+");
  if (file == NULL) {
    perror("fopen");
  }

  printf("Writing to file %s.\n", filename);

  fprintf(file, "VERTICES %d\n", num_vertices);
  fwrite(vertices, sizeof(struct vertex), num_vertices, file);
  fprintf(file, "\n");

  fprintf(file, "INDICES %d\n", num_indices);
  fwrite(indices, sizeof(uint32_t), num_indices, file);
  fprintf(file, "\n");
  fclose(file);
}

/*
 * ============================================================================
 */
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
