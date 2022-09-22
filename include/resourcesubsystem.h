#ifndef RESOURCESUBSYSTEM_H_
#define RESOURCESUBSYSTEM_H_

#include <assimp/scene.h>

#include <algorithm>
#include <vector>

#include "resource.h"
#include "types.h"

/**
 * @brief The resource management subsystem. Loads resources from files into
 *        memory.
 */
class ResourceSubsystem {
  template <typename T>
  using ResourceList = std::vector<Resource<T>>;

 public:
  /**
   * @brief Get the static instance of the subsystem.
   *
   * @return A const reference to the instance.
   */
  static ResourceSubsystem &instance() {
    static ResourceSubsystem *r = new ResourceSubsystem();
    return *r;
  }

  /**
   * @brief Load a mesh from file.
   *
   * @param filepath The path to the mesh file.
   * @return The resource.
   */
  Resource<Model> load_model(const char *filepath);

 private:
  // singleton
  ~ResourceSubsystem() = default;
  ResourceSubsystem() = default;
  ResourceSubsystem(const ResourceSubsystem &) = delete;
  ResourceSubsystem(ResourceSubsystem &&) = delete;
  ResourceSubsystem &operator=(const ResourceSubsystem &) = delete;
  ResourceSubsystem &operator=(ResourceSubsystem &&) = delete;

  ResourceList<Model> m_loaded_models;
  ResourceList<Material> m_loaded_materials;
  // ResourceList<Texture> m_loaded_textures;

  std::vector<Mesh> process_node(const char *filepath, aiNode *node,
                                 const aiScene *scene);

  /**
   * @brief Process mesh data.
   *
   * @param filepath The path to the asset.
   * @param mesh The aiMesh to process.
   * @param scene The aiScene.
   * @return Mesh data.
   */
  Mesh process_mesh(const char *filepath, aiMesh *mesh, const aiScene *scene);

  /**
   * @brief Load materials from file. This is done when the filepath
   *        points to a 3D model with the materials and images in the
   *        same directory.
   *
   * @param mat The material (assimp).
   * @return The material resource.
   */
  Resource<Material> load_materials(aiMaterial *mat, const char *filepath);

  /**
   * @brief Load image texture of a particular type from an aiMaterial. It is
   *        assumed that there is only one texture of each type.
   *
   * @param mat The aiMaterial to get the filepath from.
   * @param type The type of the image to load.
   * @return The image texture.
   */
  std::vector<Texture> load_textures(aiMaterial *mat, aiTextureType type);

  /**
   * @brief Check if a mesh resource has been loaded from a path.
   *
   * @param filepath The filepath to the resource.
   * @return An iterator to the resource.
   */
  auto mesh_loaded(const char *filepath) const;

  /**
   * @brief Check if a material resource has been loaded from a path.
   *
   * @param filepath The filepath to the resource.
   * @return An iterator to the resource.
   */
  auto material_loaded(const char *filepath) const;
};

#endif  // RESOURCESUBSYSTEM_H_
