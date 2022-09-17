#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <filesystem>
#include <memory>

/*! \brief A game resource.
 *         Manages the memory of the underying resource.
 */
template<typename ResourceType>
class Resource {
  std::unique_ptr<ResourceType> mp_resource;
  std::filesystem::path m_filepath;

  // Resource(Resource&&) = delete;
  // Resource& operator=(const Resource&) = delete;
  // Resource& operator=(Resource&&) = delete;
public:
  /*! \brief Construct a Resource. We assume the memory has not already been
   *         allocated.
   */
  Resource(ResourceType *resource, const char* filepath)
    : mp_resource(resource)
    , m_filepath(filepath) {}

  Resource(const Resource<ResourceType>&) {
    // TODO HOW
  }

  ~Resource() = default;

  /*! \brief Comparison operator. If the filepaths are the same then resources
   *         are assumed to be identical.
   *  \param r The other Resource of any templated type.
   *  \return true if the paths to the resource are the same, otherwise false.
   */
  template<typename T>
  bool operator==(const Resource<T> &r) {
    return m_filepath.string() == r.string();
  }

  /*! \brief Get a const ref to the underlying resource.
   *  \return A const reference to the resource.
   */
  const inline ResourceType& get() const { return mp_resource.get(); }

  /*! \brief Get the filepath of the resource as a string.
   *  \return The filepath string.
   */
  std::string filepath() {
    return m_filepath.string();
  }
};
#endif // RESOURCE_H_
