#ifndef ARCHETYPE_H_
#define ARCHETYPE_H_

#include "types.hpp"

#include <cstddef>
#include <stdexcept>
#include <typeindex>
#include <vector>

namespace xen
{

struct archetype_storage_base
{
  virtual void add_entity(eid_t) = 0;
  virtual void* at_offset(std::size_t offset, eid_t entity) = 0;
  virtual std::size_t num_entities() = 0;
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

  void*
  at_offset(eid_t entity, std::size_t offset) override
  {
    auto it = find_by_entity(entity);
    return &it->bytes[offset];
  };

  std::size_t
  num_entities()
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

  void add_entity(eid_t entity);

  std::size_t
  num_entities()
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
  std::unordered_map<std::type_index, std::size_t> m_offset_map;
  std::shared_ptr<archetype_storage_base> m_storage;
};

} /* end of namespace xen */

#endif /* ARCHETYPE_H_ */
