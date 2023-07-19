#ifndef CHUNK_HPP_
#define CHUNK_HPP_

#include "types.hpp"
#include <cassert>
#include <cstddef>
#include <typeindex>

namespace xen
{

/**
 * @brief A section of memory large enough to store size bytes.
 *
 * @tparam size The size of the memory chunk in bytes.
 */
template <std::size_t N>
struct chunk
{
  template <typename... ComponentTs>
  chunk(eid_t entity, ComponentTs... vals) : m_entity{ entity }
  {
    constexpr std::size_t size = (sizeof(ComponentTs) + ...);
    static_assert(size == N,
                  "Size of components is not equal to size of chunk");
    std::size_t offset = 0;
    (
        [&] {
          m_offset_map[std::type_index(typeid(ComponentTs))] = offset;
          *reinterpret_cast<ComponentTs*>(&m_bytes[offset]) = vals;
          offset += sizeof(ComponentTs);
        }(),
        ...);
  }

  /**
   * @brief Get a component from the chunk by type index.
   *
   * @throws std::runtime_exception If the type_index is not found in this
   * chunk.
   * @param index The type index.
   * @return void*
   */
  void*
  get_by_type_index(const std::type_index& index)
  {
    return &m_bytes[m_offset_map.at(index)];
  }

  /**
   * @brief Get a component from the chunk.
   *
   * @tparam Component The type of the component to get.
   * @return Component&
   */
  template <typename Component>
  Component&
  get_component()
  {
    return *reinterpret_cast<Component*>(
        get_by_type_index(std::type_index(typeid(Component))));
  }

  /**
   * @brief Get the entity that owns this chunk.
   *
   * @return eid_t
   */
  eid_t
  entity() const
  {
    return m_entity;
  }

public:
  eid_t m_entity;
  std::byte m_bytes[N];
  std::unordered_map<std::type_index, std::size_t> m_offset_map;
};

} /* end of namespace xen */

#endif /* CHUNK_HPP_ */