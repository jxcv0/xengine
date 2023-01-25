// MESHES 3
// VERTICES 1236
// <pos_x> <pos_y> <pos_z> <norm_x> <norm_y> <norm_z> <tex_x> <tex_y>
// <pos_x> <pos_y> <pos_z> <norm_x> <norm_y> <norm_z> <tex_x> <tex_y>
// ...
//
// INDICES 342
// 1 2 3 4 5 6 7 8 9 ...
//
// TEXTURES
// <diffuse_filename>
// <specular_filename>
// <normal_filename>
//
// ...

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

#include "mesh.h"

#define MAX_MESHES 16
#define MAX_TEXTURES 3

char directory[256];

unsigned int num_meshes = 0;
struct vertex *out_vertices[MAX_MESHES] = {0};
uint32_t *out_indices[MAX_MESHES] = {0};
uint32_t out_num_vertices[MAX_MESHES] = {0};
uint32_t out_num_indices[MAX_MESHES] = {0};
bool out_has_textures[MAX_MESHES] = {0};  // could be bitmask
char *out_texture_names[MAX_MESHES][MAX_TEXTURES] = {0};

FILE *file;

void process_node(struct aiNode *node, const struct aiScene *scene);
void process_mesh(struct aiMesh *mesh, const struct aiScene *scene);
char *process_material(struct aiMaterial *mat, enum aiTextureType);

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("usage:\n\tconverter <in file> <out file>\n");
    return 0;
  }

  file = fopen(argv[2], "w+");
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
  fprintf(file, "MESHES %d\n", num_meshes);

  for (unsigned int i = 0; i < num_meshes; i++) {
    fprintf(file, "VERTICES %d\n", out_num_vertices[i]);
    for (unsigned int j = 0; j < out_num_vertices[i]; j++) {
      fprintf(
          file, "%f %f %f %f %f %f %f %f\n", out_vertices[i][j].m_position[0],
          out_vertices[i][j].m_position[1], out_vertices[i][j].m_position[2],

          out_vertices[i][j].m_normal[0], out_vertices[i][j].m_normal[1],
          out_vertices[i][j].m_normal[2],

          out_vertices[i][j].m_tex_coord[0], out_vertices[i][j].m_tex_coord[1]);
    }

    fprintf(file, "INDICES %d\n", out_num_indices[i]);
    for (unsigned int j = 0; j < out_num_indices[i]; j++) {
      fprintf(file, "%d\n", out_indices[i][j]);
    }

    if (out_has_textures[i]) {
      fprintf(file, "TEXTURES\n");
      for (unsigned int j = 0; j < 3; j++) {
        fprintf(file, "%s\n", out_texture_names[i][j]);
      }
    }
  }

  aiReleaseImport(scene);
  fclose(file);
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
  out_texture_names[num_meshes][0] =
      process_material(material, aiTextureType_DIFFUSE);
  out_texture_names[num_meshes][1] =
      process_material(material, aiTextureType_SPECULAR);
  out_texture_names[num_meshes][2] =
      process_material(material, aiTextureType_HEIGHT);

  // load all or none
  if (out_texture_names[num_meshes] != NULL) {
    out_has_textures[num_meshes] = true;
  }

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

// num of meshes
// num vertices
// vertices
// texture file names
// animation data...

//
// clear_db();
//
// load_world_at_position(PLAYER_START);
//
// e1 = create_entity(); // primary key of db
// e2 = create_entity();
//
// set_health(e1, PLAYER_START_HEALTH);
//
// create_model(e1, "player_mesh"); // loads meshes, collision meshes, textures
//                                  // and animations.
// create_model(w2, "npc1_mesh");
//
// make_controlled(e1, PLAYER_INPUT); // the velocity of this entity is updated
//                                    // by user input
//
// while (game_running) {
//   load_world(camera_position);
//
//   process_input();
//   process_animations();
//   process_physics();
//
//   render_geometry();
//   render_lighting();
// }
//
