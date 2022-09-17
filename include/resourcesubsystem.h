#ifndef RESOURCESUBSYSTEM_H_
#define RESOURCESUBSYSTEM_H_

#include <vector>

#include <assimp/scene.h>

#include "types.h"
#include "resource.h"

template<typename T>
using ResourceList = std::vector<Resource<T>>;

/*! \brief Resource managment subsystem. Loads files from disk into memory.
 *         TODO async loading
 *         TODO need to know how much memory this needs
 */
class ResourceSubsystem {

  // TODO paths should be compared here and associated with data
  ResourceList<Mesh> m_loaded_meshes;
  ResourceList<Material> m_loaded_materials;
  ResourceList<Texture> m_loaded_textures;

  void process_node(const char *dir, aiNode *node, const aiScene *scene);

  void process_mesh(const char *dir, aiMesh *mesh, const aiScene *scene);

  // singleton
  ResourceSubsystem() = default;
  ResourceSubsystem(const ResourceSubsystem&) = delete;
  ResourceSubsystem(ResourceSubsystem&&) = delete;
  ResourceSubsystem& operator=(const ResourceSubsystem&) = delete;
  ResourceSubsystem& operator=(ResourceSubsystem&&) = delete;
public:
  
  /*! \brief Get the static instance of the resource subsystem.
   *  \return A reference to the subsystem.
   */
  static ResourceSubsystem& instance() {
    static ResourceSubsystem r;
    return r;
  }

  /*! \brief Default dtor
   */
  ~ResourceSubsystem() = default;

  void load_mesh(const char *filepath);

  void load_material(aiMaterial *mat,
                     aiTextureType type,
                     const char *name);

  /*! \brief Load an image from a path.
   *  \param path The filepath to the image file.
   */
	void load_image(const char* path);

  bool path_is_loaded(const char* path);
};

#endif // RESOURCESUBSYSTEM_H_
