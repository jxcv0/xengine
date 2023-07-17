#ifndef ARCHETYPE_H_
#define ARCHETYPE_H_

#include "types.hpp"

#include <cstddef>
#include <stdexcept>
#include <typeindex>
#include <vector>

namespace xen
{

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
    auto* ptr = &m_bytes[m_offset_map.at(std::type_index(typeid(Component)))];
    return *reinterpret_cast<Component*>(ptr);
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
    if (!((std::type_index(typeid(ComponentTs)) == index) || ...))
      {
        throw std::runtime_error("Type not found in archetype");
      }
    return &m_bytes[m_offset_map[index]];
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
  std::byte m_bytes[size];
  std::unordered_map<std::type_index, std::size_t> m_offset_map;
};

/**
 * @brief Interface class for archetype storage.
 * This allows archetypes to be stored in an array regardless of the size of
 * the types they store.
 *
 */
struct archetype_storage_base
{
  /**
   * @brief Get the chunk size of the archetype.
   *
   * @return std::size_t
   */
  virtual std::size_t
  chunk_size() const
  {
    return 0;
  }

  /**
   * @brief Allocate storage for a new entity.
   *
   * @param entity The entity to allocate for.
   */
  virtual void add_entity(eid_t entity) = 0;

  virtual bool
  has_entity(eid_t entity) const
  {
    (void)entity;
    return false;
  }

  virtual std::size_t
  size() const
  {
    return 0;
  }

  /**
   * @brief Get a pointer to a component by it's entity.
   * 
   * @param entity The entity that owns the component
   * @param index The type index of the component.
   * @return void* 
   */
  virtual void* get(eid_t entity, const std::type_index& index) = 0;

  /**
   * @brief Get a pointer to a chunk by its index in memory.
   * 
   * @param at The index in the array.
   * @param index The type index of the component.
   * @return void* 
   */
  virtual void* at(std::size_t at, const std::type_index& index) = 0;
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
  chunk_size() const override
  {
    return chunk<ComponentTs...>::size;
  }

  /**
   * @copydoc xen::archetype_storage_base::add_entity
   *
   */
  void
  add_entity(eid_t entity) override
  {
    m_arr.push_back(chunk<ComponentTs...>(entity));
  }

  /**
   * @copydoc xen::archetype_storage_base::has_entity
   *
   */
  bool
  has_entity(eid_t entity) const override
  {
    return find_by_entity(entity) != m_arr.cend();
  }

  std::size_t
  size() const override
  {
    return m_arr.size();
  }

  void*
  get(eid_t entity, const std::type_index& index) override
  {
    auto it = find_by_entity(entity);
    if (it == m_arr.end())
      {
        throw std::runtime_error("Entity not found in archetype");
      }
    return it->get_by_type_index(index);
  }

  void*
  at(std::size_t at, const std::type_index& index) override
  {
    return m_arr.at(at).get_by_type_index(index);
  }

private:
  auto
  find_by_entity(eid_t entity)
  {
    return std::find_if(m_arr.begin(), m_arr.end(),
                        [=](const auto& c) { return c.entity() == entity; });
  }

  auto
  find_by_entity(eid_t entity) const
  {
    return std::find_if(m_arr.cbegin(), m_arr.cend(),
                        [=](const auto& c) { return c.entity() == entity; });
  }

private:
  std::vector<chunk<ComponentTs...> > m_arr;
};

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

  /**
   * @brief Create an entity and copy it's previous component values into this
   * archetype.
   *
   * @tparam PrevCmpntTs The types of the components.
   * @param entity The entity to add this archetype.
   * @param cs The components.
   */
  template <typename... PrevCmpntTs>
  void
  add_entity(eid_t entity, PrevCmpntTs... cs)
  {
    m_storage->add_entity(entity);
    ((get<PrevCmpntTs>(entity) = cs), ...);
  }

  /**
   * @brief Check if an entity in part of the archetype.
   *
   * @param entity The entity.
   * @return true if entity is of the archetype.
   * @return false if entity is not part of the archetype.
   */
  bool
  has_entity(eid_t entity) const
  {
    return m_storage->has_entity(entity);
  }

  /**
   * @brief Get the number of entities of this archetype.
   *
   * @return std::size_t
   */
  std::size_t
  size() const
  {
    return m_storage->size();
  }

  /**
   * @brief Access a component belonging to an entity.
   *
   * @tparam Component The type of the component to get.
   * @param entity The entity.
   * @return Component&
   */
  template <typename Component>
  Component&
  get(eid_t entity)
  {
    auto* ptr = m_storage->get(entity, std::type_index(typeid(Component)));
    return *reinterpret_cast<Component*>(ptr);
  }

  template <typename Component>
  Component&
  at(std::size_t index)
  {
    auto* ptr = m_storage->at(index, std::type_index(typeid(Component)));
    return *reinterpret_cast<Component*>(ptr);
  }

  /*
  auto
  begin()
  {
    return m_storage->begin();
  }

  auto
  end()
  {
    return m_storage->end();
  }
  */

private:
  /* We want to create archetypes with the static create function only */
  archetype(archetype_storage_base* base) : m_storage{ base } {}

private:
  std::unique_ptr<archetype_storage_base> m_storage;
};

} /* end of namespace xen */

#endif /* ARCHETYPE_H_ */
