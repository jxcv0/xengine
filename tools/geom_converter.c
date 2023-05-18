#include <glad.h>
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

/**
 * @brief This is the file that contains the relative filepaths of all the
 * meshes, textures and animations that make up the model.
 */
static FILE *infile;
static FILE *outfile;

void process_node(struct aiNode *node, const struct aiScene *scene);
void process_mesh(struct aiMesh *mesh, const struct aiScene *scene);
char *process_material(struct aiMaterial *mat, enum aiTextureType);

int main(int argc, char *argv[]) {
  if (argc != 3) {
      printf("Usage:\n\tgeom_converter <infile> <outfile>\n");
      exit(EXIT_FAILURE);
  }

  infile = fopen(argv[1], "rb");
  if (infile == NULL) {
      perror("Unable to open input file");
      exit(EXIT_FAILURE);
  }

  outfile = fopen(argv[2], "wb+");
  if (outfile == NULL) {
      perror("Unable to open output file");
      exit(EXIT_FAILURE);
  }

  const struct aiScene *scene =
      aiImportFile(argv[1], aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                                aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

  if (scene == NULL) {
    fprintf(stderr, "%s\n", aiGetErrorString());
    return 0;
  }

  process_node(scene->mRootNode, scene);

  aiReleaseImport(scene);
  fclose(infile);
  return 0;
}

void process_node(struct aiNode *node, const struct aiScene *scene) {
  if (node->mNumMeshes != 1) {
      fprintf(stderr, "Only single mesh models supported. %u found.\n", node->mNumMeshes);
      exit(EXIT_FAILURE);
  }

  struct aiMesh *m = scene->mMeshes[node->mMeshes[0]];
  process_mesh(m, scene);
}

void process_mesh(struct aiMesh *mesh, const struct aiScene *scene) {
  (void) scene;
  size_t num_vertices = mesh->mNumVertices;
  size_t num_indices = mesh->mNumFaces * 3;


  size_t vertices_size = sizeof(struct vertex) * num_vertices;
  size_t indices_size = sizeof(GLuint) * num_indices;
  struct vertex *vertices = malloc(vertices_size);
  GLuint *indices = malloc(indices_size);

  char *mesh_name = mesh->mName.data;

  printf("Loading mesh: %s.\n", mesh_name);
  printf("Copying %d vertices.\n", mesh->mNumVertices);

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    struct vertex *v = &vertices[i];

    v->position[0] = mesh->mVertices[i].x;
    v->position[1] = mesh->mVertices[i].y;
    v->position[2] = mesh->mVertices[i].z;

    // if no texture coords are specified in the source file this will fail.
    assert(mesh->mTangents != NULL);

    v->tangent[0] = mesh->mTangents[i].x;
    v->tangent[1] = mesh->mTangents[i].y;
    v->tangent[2] = mesh->mTangents[i].z;

    v->bitangent[0] = mesh->mBitangents[i].x;
    v->bitangent[1] = mesh->mBitangents[i].y;
    v->bitangent[2] = mesh->mBitangents[i].z;

    v->normal[0] = mesh->mNormals[i].x;
    v->normal[1] = mesh->mNormals[i].y;
    v->normal[2] = mesh->mNormals[i].z;

    if (mesh->mTextureCoords[0] != 0) {
      v->tex_coord[0] = mesh->mTextureCoords[0][i].x;
      v->tex_coord[1] = mesh->mTextureCoords[0][i].y;
    } else {
      // TODO we might want invisible meshes?
      v->tex_coord[0] = 0;
      v->tex_coord[1] = 0;
    }
  }

  printf("Copying indices from %d faces.\n", mesh->mNumFaces);
  for (unsigned int i = 0, j = 0; i < mesh->mNumFaces; i++) {
    struct aiFace face = mesh->mFaces[i];
    for (unsigned int k = 0; k < face.mNumIndices; k++) {
      indices[j++] = face.mIndices[k];
    }
  }

  printf("Writing to output file...\n");

  fprintf(outfile, "VERTICES %ld\n", vertices_size);
  fwrite(vertices, sizeof(struct vertex), num_vertices, outfile);
  fprintf(outfile, "\n");

  fprintf(outfile, "INDICES %ld\n", indices_size);
  fwrite(indices, sizeof(GLuint), num_indices, outfile);
  fprintf(outfile, "\n");
}
