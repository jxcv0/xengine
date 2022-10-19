#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <filesystem>
#include <memory>

/**
 * @brief A game resource.
 *        Manages the memory of the underying resource via a shared pointer.
 */
template <typename ResourceType, typename Allocator>
class Resource {
 public:
  /**
   * @brief Constructvencs a Resource.
   */
  Resource(const std::filesystem::path filepath,
           std::shared_ptr<Allocator> allocator)
      : mp_resource(nullptr), m_filepath(filepath), mp_allocator(allocator) {}

  Resource(const Resource<ResourceType, Allocator> &r) = default;
  Resource &operator=(const Resource &) = default;
  Resource &operator=(Resource &&) = default;
  ~Resource() = default;

  /**
   * @brief Comparison operator. If the filepaths are the same then resources
   *        are assumed to be identical.
   *
   * @param r The other Resource of any templated type.
   * @return true if the paths to the resource are the same, otherwise false.
   */
  template <typename T>
  bool operator==(const Resource<T, Allocator> &r) {
    return m_filepath == r.filepath();
  }

  /**
   * @brief Get a const ref to the underlying resource.
   *
   * @return A const reference to the resource.
   */
  const inline ResourceType &get() const { return mp_resource.get(); }

  /**
   * @brief Get the filepath of the resource.
   *
   * @return The filepath.
   */
  std::filesystem::path filepath() const { return m_filepath; }

 private:
  std::shared_ptr<ResourceType> mp_resource;
  std::filesystem::path m_filepath;
  std::shared_ptr<Allocator> mp_allocator;
};

#endif  // RESOURCE_H_
