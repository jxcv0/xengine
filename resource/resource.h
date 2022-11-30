#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <alloca.h>

#include <filesystem>
#include <memory>

#include "import.h"

/**
 * @brief A game resource.
 *        Manages the memory of the underying resource via a shared pointer.
 */
template <typename ResourceType,
          template <typename> typename Allocator = std::allocator>
class Resource {
 public:
  /**
   * @brief Constructs a Resource.
   *
   * @param filepath The path to the file the resource can be loaded from.
   */
  Resource(const std::filesystem::path filepath)
      : m_allocator(), m_filepath(filepath) {
    mp_resource = m_allocator.allocate(1);
    xen::import(mp_resource, filepath);
  }

  Resource(const Resource<ResourceType, Allocator> &r) = default;
  Resource &operator=(const Resource &) = default;
  Resource &operator=(Resource &&) = default;

  ~Resource() { m_allocator.deallocate(mp_resource, 1); }

  /**
   * @brief Comparison operator. If the filepaths are the same then resources
   *        are assumed to be identical.
   *
   * @param r The other Resource object.
   * @return true if the paths to the resource are the same, otherwise false.
   */
  template <typename T, template <typename> typename A>
  bool operator==(const Resource<T, A> &r) {
    return m_filepath == r.filepath();
  }

  /**
   * @brief Get a pointer to the underlying resource.
   *
   * @return A shared pointer to the resource.
   */
  const auto get() const { return mp_resource; }

  /**
   * @brief Get the filepath of the resource.
   *
   * @return The filepath.
   */
  std::filesystem::path filepath() const { return m_filepath; }

 private:
  ResourceType *mp_resource;
  Allocator<ResourceType> m_allocator;
  std::filesystem::path m_filepath;
};

#endif  // RESOURCE_H_
