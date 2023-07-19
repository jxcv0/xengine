#ifndef ARCHETYPE_H_
#define ARCHETYPE_H_

#include "types.hpp"

#include <atomic>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <typeindex>
#include <vector>

namespace xen
{

struct archetype_storage_base
{
  virtual void add_entity(eid_t) = 0;
  virtual void remove_entity(eid_t) = 0;
  virtual bool has_entity(eid_t) const = 0;
  virtual void* at_offset(std::size_t, eid_t) = 0;
  virtual std::size_t num_entities() const = 0;
};

template <std::size_t N>
class archetype_storage : public archetype_storage_base
{
public:
  void
  add_entity(eid_t entity) override
  {
    chunk c;
    c.entity = entity;
    m_chunks.push_back(c);
  }

  void
  remove_entity(eid_t entity) override
  {
    m_chunks.erase(find_by_entity(entity));
  }

  bool
  has_entity(eid_t entity) const override
  {
    return find_by_entity(entity) != m_chunks.end();
  }

  void*
  at_offset(eid_t entity, std::size_t offset) override
  {
    auto it = find_by_entity(entity);
    if (it == m_chunks.end())
      {
        throw std::out_of_range("Entity not found in archetype");
      }
    return &it->bytes[offset];
  };

  std::size_t
  num_entities() const
  {
    return m_chunks.size();
  }

private:
  auto
  find_by_entity(eid_t entity)
  {
    return std::find_if(m_chunks.begin(), m_chunks.end(),
                        [=](const auto& c) { return c.entity == entity; });
  }

  auto
  find_by_entity(eid_t entity) const
  {
    return std::find_if(m_chunks.cbegin(), m_chunks.cend(),
                        [=](const auto& c) { return c.entity == entity; });
  }

private:
  struct chunk
  {
    eid_t entity;
    std::byte bytes[N];
  };

  std::vector<chunk> m_chunks;
};

class archetype
{
public:
  using offset_map_t = std::unordered_map<std::type_index, std::size_t>;

  template <typename... ComponentTs>
  archetype(eid_t entity, ComponentTs... vals)
      : m_offset_map{},
        m_storage{ new archetype_storage<(sizeof(ComponentTs) + ...)> }
  {
    /* Put byte offsets in offset map */
    std::size_t offset = 0;
    (
        [&] {
          m_offset_map[std::type_index(typeid(ComponentTs))] = offset;
          offset += sizeof(ComponentTs);
        }(),
        ...);

    /* Copy values into storage using offsets */
    m_storage->add_entity(entity);
    ((*reinterpret_cast<ComponentTs*>(m_storage->at_offset(
          entity, m_offset_map[std::type_index(typeid(ComponentTs))]))
      = vals),
     ...);
  }

  template <typename... ComponentTs>
  void
  add_entity(eid_t entity, ComponentTs... vals)
  {
    m_storage->add_entity(entity);
    ((get_component<ComponentTs>(entity) = vals), ...);
  }

  void
  remove_entity(eid_t entity)
  {
    m_storage->remove_entity(entity);
  }

  template <typename Component>
  bool
  has_component() const
  {
    auto it = std::find_if(
        m_offset_map.cbegin(), m_offset_map.cend(), [](const auto& kv) {
          return kv.first == std::type_index(typeid(Component));
        });
    return it != m_offset_map.cend();
  }

  bool
  has_entity(eid_t entity) const
  {
    return m_storage->has_entity(entity);
  }

  std::size_t
  num_entities() const
  {
    return m_storage->num_entities();
  }

  void*
  get_component(eid_t entity, const std::type_index& type_index)
  {
    return m_storage->at_offset(entity, m_offset_map[type_index]);
  }

  template <typename Component>
  Component&
  get_component(eid_t entity)
  {
    return *reinterpret_cast<Component*>(
        get_component(entity, std::type_index(typeid(Component))));
  }

private:
  offset_map_t m_offset_map; /* first - offset, second - size */
  std::shared_ptr<archetype_storage_base> m_storage;
};

} /* end of namespace xen */

#endif /* ARCHETYPE_H_ */
