#include "assets.h"

#include "stb_image.h"
#include <assert.h>
#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string.h>

int
asset_type (const char *path)
{
  char *ext = strrchr (path, '.');

  if (ext == NULL)
    {
      return -1;
    }

  const char *supported_asset_exts[] = { ".mesh", ".tex" };
  for (int i = 0; i < 2; i++)
    {
      if (strcmp (supported_asset_exts[i], ext) == 0)
        {
          return i;
        }
    }

  return -1;
}

struct texture *
load_texture (const char *filepath, allocator alloc_tex)
{
  struct texture *tex = alloc_tex (sizeof (*tex));
  tex->data = stbi_load (filepath, &tex->sizeinfo.width, &tex->sizeinfo.height,
                         &tex->sizeinfo.nchannels, 0);
  if (tex->data == NULL)
    {
      return NULL;
    }
  return 0;
}

static void
process_aiMesh (struct model *model, struct aiMesh *aimesh)
{
}

static void
process_aiNode (struct model *model, const struct aiNode *node,
                const struct aiScene *scene, allocator alloc_mesh)
{
  /*
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
  */
}

struct model
load_model_assimp (const char *filepath, allocator alloc_mesh)
{
  struct model model;
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
