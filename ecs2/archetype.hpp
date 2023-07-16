#ifndef ARCHETYPE_H_
#define ARCHETYPE_H_

#include "types.hpp"
#include <cstddef>
#include <cstdint>
#include <typeindex>
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

  /**
   * @brief Construct a new chunk object assigned to an entity. Also populate
   * the offset map with byte offsets for each type.
   *
   * @param entity The entity that owns this chunk.
   */
  chunk(eid_t entity) : m_entity{ entity }
  {
    std::size_t offset = 0;
    (
        [&] {
          m_offset_map[std::type_index(typeid(ComponentTs))] = offset;
          offset += sizeof(ComponentTs);
        }(),
        ...);
  }

  template <typename Component>
  Component&
  get_component()
  {
    auto* ptr = &bytes[m_offset_map[std::type_index(typeid(Component))]];
    return *reinterpret_cast<Component*>(ptr);
  }

  eid_t
  entity() const
  {
    return m_entity;
  }

public:
  eid_t m_entity;
  std::byte bytes[size];
  std::unordered_map<std::type_index, std::size_t> m_offset_map;
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
   * @brief Construct a new archetype with storage for all types in a parameter
   * pack. All types in the parameter pack mush be unique.
   *
   * @tparam Component The first component type of the parameter pack.
   * @tparam ComponentTs The rest of the component types of the parameter pack.
   * @return archetype The new archetype.
   */
  template <typename Component, typename... ComponentTs>
  static archetype
  create()
  {
    static_assert((!std::is_same_v<Component, ComponentTs> && ...),
                  "Duplicate types in archetype");
    return archetype(new archetype_storage<Component, ComponentTs...>);
  }

  /**
   * @brief Get the size of the memory chunk required to store one instance of
   * each type.
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
