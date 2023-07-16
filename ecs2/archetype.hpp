#ifndef ARCHETYPE_H_
#define ARCHETYPE_H_

#include "types.hpp"
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <vector>

namespace xen
{

/**
 * @brief Interface class for archetype storage.
 * This allows archetypes to be stored in an array regardless of the size of
 * the types they store.
 *
 */
struct archetype_storage_base
{
  virtual std::size_t
  chunk_size() const
  {
    return 0;
  }
};

/**
 * @brief A section of memory large enough to store size bytes.
 *
 * @tparam size The size of the memory chunk in bytes.
 */
template <typename... ComponentTs>
struct chunk
{
  static const std::size_t size = (sizeof(ComponentTs) + ...);
  std::byte bytes[size];
};

/**
 * @brief Contiguous storage of components of different types.
 *
 * @tparam ComponentTs The types of the archetype.
 */
template <typename... ComponentTs>
class archetype_storage : public archetype_storage_base
{
public:
  /**
   * @brief Get the size of the chucks required
   *
   * @return std::size_t
   */
  virtual std::size_t
  chunk_size() const
  {
    return chunk<ComponentTs...>::size;
  }

private:
  std::vector<chunk<ComponentTs...> > m_arr;
};

/**
 * @brief TODO
 *
 */
class archetype
{
public:
  /**
   * @brief Create an archetype.
   *
   * @tparam ComponentTs The types that all entities of this archetype have an
   * instance of.
   * @return archetype The new archetype.
   */
  template <typename Component, typename... ComponentTs>
  static archetype
  create()
  {
    static_assert((!std::is_same_v<Component, ComponentTs> && ...), "Duplicate types in archetype");
    return archetype(new archetype_storage<Component, ComponentTs...>);
  }

  /**
   * @brief Get the size of the memory chunk required to store one instance of
   * each type
   *
   * @return std::size_t
   */
  std::size_t
  chunk_size()
  {
    return m_storage->chunk_size();
  }

private:
  archetype(archetype_storage_base* base) noexcept : m_storage{ base } {}

private:
  std::unique_ptr<archetype_storage_base> m_storage;
};

} /* end of namespace xen */

#endif /* ARCHETYPE_H_ */
