#include <assert.h>
#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdio.h>
#include <stdlib.h>

#include "mesh.h"

#define MAX_MESHES 16

unsigned int num_meshes = 0;
struct vertex *vertices[MAX_MESHES];

// TODO struct animation dest_anim[MAX_ANIMATIONS];

void process_node(struct aiNode *node, const struct aiScene *scene);
void process_mesh(struct aiMesh *mesh, const struct aiScene *scene);

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("usage:\n\tconverter <in file> <out file>\n");
    return 0;
  }
  const struct aiScene *scene =
      aiImportFile(argv[1], aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                                aiProcess_JoinIdenticalVertices);
  if (scene == NULL) {
    fprintf(stderr, "%s\n", aiGetErrorString());
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
  assert(num_meshes < MAX_MESHES);
  vertices[num_meshes++] = malloc(mesh->mNumVertices);
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
  }
}

// num of meshes
// num vertices
// vertices
// texture file names
// animation data...
