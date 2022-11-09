#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <filesystem>
#include <memory>

#include "importer.h"

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
      : mp_resource(nullptr), m_filepath(filepath) {}

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
  template <typename T, template <typename> typename A>
  bool operator==(const Resource<T, A> &r) {
    return m_filepath == r.filepath();
  }

  /**
   * @brief Get a const ref to the underlying resource.
   *
   * @return A shared pointer to the resource.
   */
  auto get() {
    if (mp_resource == nullptr) {
      Importer<ResourceType> importer(m_filepath);
      mp_resource.reset(importer.import());
    }
    return mp_resource;
  }

  /**
   * @brief Get the filepath of the resource.
   *
   * @return The filepath.
   */
  std::filesystem::path filepath() const { return m_filepath; }

 private:
  std::shared_ptr<ResourceType> mp_resource;
  std::filesystem::path m_filepath;
};

#endif  // RESOURCE_H_
