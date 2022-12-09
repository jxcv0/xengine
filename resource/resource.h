#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <filesystem>

#include "mallocator.h"

/**
 * @brief Manages the memory of a game resource
 */
template <typename ResourceType,
          template <typename> typename Alloc = Mallocator>
class Resource {
 public:
  /**
   * @brief Constructs a Resource.
   *
   * @param filepath The path to the file the resource can be loaded from.
   */
  Resource(const std::filesystem::path filepath)
      : m_alloc(), m_filepath(filepath), mp_resource(nullptr) {
    mp_resource = m_alloc.allocate(1);
  }

  Resource(const Resource<ResourceType, Alloc> &r) = default;
  Resource &operator=(const Resource &) = default;
  Resource &operator=(Resource &&) = default;

  ~Resource() {
    mp_resource.unload(m_alloc);
    m_alloc.deallocate(mp_resource, 1);
  }

  /**
   * @brief Comparison operator. If the filepaths are the same then resources
   *        are assumed to be identical.
   *
   * @param r The other Resource object.
   * @return true if the paths to the resource are the same, otherwise false.
   */
  bool operator==(const Resource &r) { return m_filepath == r.filepath(); }

  /**
   * @brief Get a pointer to the underlying resource.
   *
   * @return A pointer to the resource.
   */
  auto get() const { return mp_resource; }

  /**
   * @brief Get the filepath of the resource.
   *
   * @return The filepath.
   */
  std::filesystem::path filepath() const { return m_filepath; }

 private:
  ResourceType mp_resource;
  Alloc<ResourceType> m_alloc;
  std::filesystem::path m_filepath;
};

#endif  // RESOURCE_H_
