#ifndef RESOURCESUBSYSTEM_H_
#define RESOURCESUBSYSTEM_H_

#include <algorithm>
#include <vector>

#include <assimp/scene.h>

#include "types.h"
#include "resource.h"

/**
 * @brief The resource management subsystem. Loads resources from files into
 *        memory.
 */
class ResourceSubsystem {
  template<typename T>
  using ResourceList = std::vector<Resource<T>>;
public:
  
  ResourceSubsystem() = default;

  /**
   * @brief Get the static instance of the subsystem.
   * 
   * @return A const reference to the instance.
   */
  static ResourceSubsystem& instance() {
    static ResourceSubsystem r;
    return r;
  }

  /** @brief Default destructor.
   */
  ~ResourceSubsystem() = default;

  /**
   * @brief Load a mesh from file.
   * 
   * @param filepath The path to the mesh file.
   * @return The resource.
   */
  Resource<Model> load_model(const char *filepath);

  /**
   * @brief Load an texture image file.
   * 
   * @param filepath The filepath to the resource.
   * @return The image resource.
   */
  Resource<Texture> load_image(const char* filepath);

  /**
   * @brief Check if a mesh resource has been loaded from a path.
   * 
   * @param filepath The filepath to the resource.
   * @return An iterator to the resource.
   */
  auto mesh_loaded(const char* filepath) const {
    return std::find_if(m_loaded_models.begin(), m_loaded_models.end(),
        [=](auto &r){ r.filepath() == filepath; });
  }

private:
  ResourceList<Model> m_loaded_models;
  ResourceList<Material> m_loaded_materials;
  ResourceList<Texture> m_loaded_textures;

  std::vector<Mesh> process_node(const char *filepath,
                                  aiNode *node,
                                  const aiScene *scene);

  Mesh process_mesh(const char *filepath,
                              aiMesh *mesh,
                              const aiScene *scene);

  /**
   * @brief Load a material from file. This is done when the filepath
   *        points to a 3D model with the materials and images in the
   *        same directory.
   * 
   * @param mat The material (assimp).
   * @param type The aiTextureType.
   * @param name The na
   * @return Resource<Material> 
   */
  Resource<Material> load_material(aiMaterial *mat,
                     aiTextureType type,
                     const char *name);

  // singleton
  ResourceSubsystem(const ResourceSubsystem&) = delete;
  ResourceSubsystem(ResourceSubsystem&&) = delete;
  ResourceSubsystem& operator=(const ResourceSubsystem&) = delete;
  ResourceSubsystem& operator=(ResourceSubsystem&&) = delete;
};

#endif // RESOURCESUBSYSTEM_H_
