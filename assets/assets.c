#include "assets.h"

#include <assert.h>
#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string.h>

#include "stb_image.h"

struct mesh_allocator
create_mesh_allocator(int bufsize)
{
  void* mem = malloc(bufsize * sizeof(struct mesh) + bufsize * sizeof(int));
  struct mesh_allocator alloc = { .buf = mem,
                                  .free = mem + (bufsize * sizeof(int)),
                                  .bufsize = bufsize,
                                  .nmeshes = 0,
                                  .nfree = bufsize };
  sem_init(&alloc.sem, 0, 1);
  for (int i = 0; i < bufsize; i++)
    {
      alloc.free[i] = bufsize - (i + 1);
    }
  return alloc;
}

int
alloc_mesh(struct mesh_allocator* alloc)
{
  if (sem_wait(&alloc->sem) == -1)
    {
      return -1;
    }

  if (alloc->nmeshes == alloc->bufsize)
    {
      sem_post(&alloc->sem);
      return -1;
    }

  int index = alloc->free[--alloc->nfree];
  ++alloc->nmeshes;
  sem_post(&alloc->sem);
  return index;
}

int
free_mesh(struct mesh_allocator* alloc, int index)
{
  if (sem_wait(&alloc->sem) == -1)
    {
      return -1;
    }

  if (alloc->nmeshes == 0)
    {
      sem_post(&alloc->sem);
      return -1;
    }

  alloc->free[alloc->nfree++] = index;
  --alloc->nmeshes;
  sem_post(&alloc->sem);
  return 0;
}

int
asset_type(const char* path)
{
  char* ext = strrchr(path, '.');

  if (ext == NULL)
    {
      return -1;
    }

  const char* supported_asset_exts[] = { ".mesh", ".tex" };
  for (int i = 0; i < 2; i++)
    {
      if (strcmp(supported_asset_exts[i], ext) == 0)
        {
          return i;
        }
    }

  return -1;
}

struct texture
load_texture(const char* filepath)
{
  struct texture tex;
  tex.data = stbi_load(filepath, &tex.sizeinfo.width, &tex.sizeinfo.height,
                       &tex.sizeinfo.nchannels, 0);
  return tex;
}

/*
static void
process_aiMesh (struct model *model, struct aiMesh *aimesh)
{
}

static void
process_aiNode (struct model *model, const struct aiNode *node,
                const struct aiScene *scene, allocator alloc_mesh)
{
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
      process_aiMesh (meshes[i], scene->mMeshes[node->mMeshes[i]]);
    }

  for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
      process_aiNode
    }

  model->meshes = meshes;
  model->nmeshes = node->mNumMeshes;
}

struct model
load_model_assimp (const char *filepath, allocator alloc_mesh)
{
  const struct aiScene *scene
      = aiImportFile (filepath, aiProcess_GenSmoothNormals | aiProcess_FlipUVs
                                    | aiProcess_CalcTangentSpace);

  if (scene != NULL)
    {
      process_aiNode (&model, scene->mRootNode, scene, alloc_mesh);
    }

  aiReleaseImport (scene);
  return model;
}

struct model
load_meshes (const char *filepath, allocator alloc_mesh)
{
  return load_model_assimp (filepath, alloc_mesh);
}
*/
